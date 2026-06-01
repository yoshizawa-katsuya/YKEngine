#include "object3d.hlsli"

struct Material
{
    float32_t4 color;
    bool enableLighting;
    float32_t shininess;
    float32_t enviromentCoefficient; //環境マップの影響度
    float32_t4x4 uvTransform;
    
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

struct PointLight
{
    float32_t4 color; //ライトの色
    float32_t3 position; //ライトの位置
    float intensity; //輝度
    float radius; //ライトの届く最大距離
    float decay; //減衰率
    float padding[2];
};

struct SpotLight
{
    float32_t4 color; //ライトの色
    float32_t3 position; //ライトの位置
    float32_t intensity; //輝度
    float32_t3 direction; //ライトの向き
    float32_t distance; //ライトの届く最大距離
    float32_t decay; //減衰率
    float32_t cosAngle; //スポットライトの余弦
    float32_t cosFalloffStart; //falloff開始の角度
    float padding;
};

struct LightCount
{
    uint directional;
    uint points;
    uint spot;
};

struct Camera
{
    float32_t3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);
StructuredBuffer<DirectionalLight> gDirectionalLight : register(t1);
StructuredBuffer<PointLight> gPointLight : register(t2);
StructuredBuffer<SpotLight> gSpotLight : register(t3);
ConstantBuffer<Camera> gCamera : register(b1);
ConstantBuffer<LightCount> gLightCount : register(b2);
Texture2D<float32_t4> gTexture : register(t0);
TextureCube<float32_t4> gEnvironmentTexture : register(t4);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

/// <summary>
/// トリプラナーマッピングを行う関数
/// </summary>
/// <param name="worldPosition">ワールド空間での位置</param>
/// <param name="normal">法線</param>
/// <returns>テクスチャカラー</returns>
float4 TriplanarMapping(float32_t3 worldPosition, float32_t3 normal)
{
    float scale = 0.1f; //テクスチャのスケール
    
    float2 uvX = worldPosition.yz * scale;
    float2 uvY = worldPosition.xz * scale;
    float2 uvZ = worldPosition.xy * scale;
    
    float4 colorX = gTexture.Sample(gSampler, uvX);
    float4 colorY = gTexture.Sample(gSampler, uvY);
    float4 colorZ = gTexture.Sample(gSampler, uvZ);
    
    float3 absNormal = abs(normal);
    float3 blendWeights = pow(absNormal, 5.0f); //ブレンドの重みを計算
    blendWeights /= (blendWeights.x + blendWeights.y + blendWeights.z); //正規化
    
    return blendWeights.x * colorX + blendWeights.y * colorY + blendWeights.z * colorZ;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float4 textureColor = TriplanarMapping(input.worldPosition, input.normal);
    float32_t3 nomalizedNormal = normalize(input.normal);

    //textureのα値が0のときにPixelを棄却
    if (textureColor.a <= 0.0)
    {
        discard;
    }
    
    if (gMaterial.enableLighting != 0)
    {
        
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        
        float NdotL, cos, NDotH, specularPow;
        uint count;
        float32_t3 halfVector, diffuseDirectionalLight, specularDirectionalLight;
        diffuseDirectionalLight = float32_t3(0.0f, 0.0f, 0.0f);
        specularDirectionalLight = float32_t3(0.0f, 0.0f, 0.0f);
        
        for (int i = 0; i < gLightCount.directional; i++)
        {
            NdotL = dot(nomalizedNormal, -gDirectionalLight[i].direction);
            cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            
            halfVector = normalize(-gDirectionalLight[i].direction + toEye);
            NDotH = dot(nomalizedNormal, halfVector);
            specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
            //拡散反射
            diffuseDirectionalLight += gMaterial.color.rgb * textureColor.rgb * gDirectionalLight[i].color.rgb * cos * gDirectionalLight[i].intensity;
        
            //鏡面反射
            specularDirectionalLight += gDirectionalLight[i].color.rgb * gDirectionalLight[i].intensity * specularPow;
        
        }
        
        float32_t3 pointLightDirection, diffusePointLight, specularPointLight;
        float32_t distance, factor;
        diffusePointLight = float32_t3(0.0f, 0.0f, 0.0f);
        specularPointLight = float32_t3(0.0f, 0.0f, 0.0f);
        
        //ポイントライト
        for (int j = 0; j < gLightCount.points; j++)
        {
            pointLightDirection = normalize(input.worldPosition - gPointLight[j].position);
        
            halfVector = normalize(-pointLightDirection + toEye);
            NDotH = dot(nomalizedNormal, halfVector);
            specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
            distance = length(gPointLight[j].position - input.worldPosition); //ポイントライトへの距離
            factor = pow(saturate(-distance / gPointLight[j].radius + 1.0), gPointLight[j].decay); //指数によるコントロール
        
            diffusePointLight += gMaterial.color.rgb * textureColor.rgb * gPointLight[j].color.rgb * gPointLight[j].intensity * factor;
            specularPointLight += gPointLight[j].color.rgb * gPointLight[j].intensity * factor * specularPow;
        
        }
        
        float32_t3 spotLightDirectionOnSurface, diffuseSpotLight, specularSpotLight;
        float32_t attenuationfactor, cosAngle, falloffFactor;
        diffuseSpotLight = float32_t3(0.0f, 0.0f, 0.0f);
        specularSpotLight = float32_t3(0.0f, 0.0f, 0.0f);
        
        //スポットライト
        for (int k = 0; k < gLightCount.spot; k++)
        {
            spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight[k].position);
        
            halfVector = normalize(-spotLightDirectionOnSurface + toEye);
            NDotH = dot(nomalizedNormal, halfVector);
            specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
            distance = length(gSpotLight[k].position - input.worldPosition); //スポットライトへの距離
            attenuationfactor = pow(saturate(-distance / gSpotLight[k].distance + 1.0), gSpotLight[k].decay); //指数によるコントロール
        
            cosAngle = dot(spotLightDirectionOnSurface, gSpotLight[k].direction);
            falloffFactor = saturate((cosAngle - gSpotLight[k].cosAngle) / (gSpotLight[k].cosFalloffStart - gSpotLight[k].cosAngle));
        
            diffuseSpotLight += gMaterial.color.rgb * textureColor.rgb * gSpotLight[k].color.rgb * gSpotLight[k].intensity * attenuationfactor * falloffFactor;
            specularSpotLight += gSpotLight[k].color.rgb * gSpotLight[k].intensity * attenuationfactor * falloffFactor * specularPow;
        
        }
        
        //拡散反射+鏡面反射
        output.color.rgb = diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
        
        //アルファは今まで通り
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    
    if (gMaterial.enviromentCoefficient != 0.0f)
    {
        //環境マップの適用
        float32_t3 cameraToPosition = normalize(input.worldPosition - gCamera.worldPosition);
        float32_t3 reflectedVector = reflect(cameraToPosition, nomalizedNormal);
        float32_t4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);
        
        output.color.rgb += environmentColor.rgb * gMaterial.enviromentCoefficient;
    }
    
    
    //output.colorのα値が0のときPixelを棄却
    if (output.color.a == 0.0)
    {
        discard;
    }
    return output;
}