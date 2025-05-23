#include "object3d.hlsli"

struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
    float32_t shininess;
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
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float4 transformedUV = mul(float32_t4(input.texcord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
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
            NdotL = dot(normalize(input.normal), -gDirectionalLight[i].direction);
            cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            
            halfVector = normalize(-gDirectionalLight[i].direction + toEye);
            NDotH = dot(normalize(input.normal), halfVector);
            specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
            //拡散反射
            diffuseDirectionalLight += gMaterial.color.rgb * textureColor.rgb * gDirectionalLight[i].color.rgb * cos * gDirectionalLight[i].intensity;
        
            //鏡面反射
            specularDirectionalLight += gDirectionalLight[i].color.rgb * gDirectionalLight[i].intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        
        }
        /*
        NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        */
        //half lambert
        /*
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        */
        
        
        //フォン反射モデル
        /*
        float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
        float RdotE = dot(reflectLight, toEye);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess);  //反射強度
        */
        /*
        //ブリン・フォン反射モデル
        //平行光源
        float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
        //拡散反射
        float32_t3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        //鏡面反射
        float32_t3 specularDirectionalLight = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        */
        
        float32_t3 pointLightDirection, diffusePointLight, specularPointLight;
        float32_t distance, factor;
        diffusePointLight = float32_t3(0.0f, 0.0f, 0.0f);
        specularPointLight = float32_t3(0.0f, 0.0f, 0.0f);
        
        //ポイントライト
        for (int j = 0; j < gLightCount.points; j++)
        {
            pointLightDirection = normalize(input.worldPosition - gPointLight[j].position);
        
            halfVector = normalize(-pointLightDirection + toEye);
            NDotH = dot(normalize(input.normal), halfVector);
            specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
            distance = length(gPointLight[j].position - input.worldPosition); //ポイントライトへの距離
            factor = pow(saturate(-distance / gPointLight[j].radius + 1.0), gPointLight[j].decay); //指数によるコントロール
        
            diffusePointLight += gMaterial.color.rgb * textureColor.rgb * gPointLight[j].color.rgb * gPointLight[j].intensity * factor;
            specularPointLight += gPointLight[j].color.rgb * gPointLight[j].intensity * factor * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        
        }
        
        /*
        float32_t3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        
        halfVector = normalize(-pointLightDirection + toEye);
        NDotH = dot(normalize(input.normal), halfVector);
        specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
        float32_t distance = length(gPointLight.position - input.worldPosition); //ポイントライトへの距離
        float32_t factor = pow(saturate(-distance / gPointLight.radius + 1.0), gPointLight.decay); //指数によるコントロール
        
        float32_t3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * gPointLight.intensity * factor;
        float32_t3 specularPointLight = gPointLight.color.rgb * gPointLight.intensity * factor * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        */
        
        float32_t3 spotLightDirectionOnSurface, diffuseSpotLight, specularSpotLight;
        float32_t attenuationfactor, cosAngle, falloffFactor;
        diffuseSpotLight = float32_t3(0.0f, 0.0f, 0.0f);
        specularSpotLight = float32_t3(0.0f, 0.0f, 0.0f);
        
        //スポットライト
        for (int k = 0; k < gLightCount.spot; k++)
        {
            spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight[k].position);
        
            halfVector = normalize(-spotLightDirectionOnSurface + toEye);
            NDotH = dot(normalize(input.normal), halfVector);
            specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
            distance = length(gSpotLight[k].position - input.worldPosition); //スポットライトへの距離
            attenuationfactor = pow(saturate(-distance / gSpotLight[k].distance + 1.0), gSpotLight[k].decay); //指数によるコントロール
        
            cosAngle = dot(spotLightDirectionOnSurface, gSpotLight[k].direction);
            falloffFactor = saturate((cosAngle - gSpotLight[k].cosAngle) / (gSpotLight[k].cosFalloffStart - gSpotLight[k].cosAngle));
        
            diffuseSpotLight += gMaterial.color.rgb * textureColor.rgb * gSpotLight[k].color.rgb * gSpotLight[k].intensity * attenuationfactor * falloffFactor;
            specularSpotLight += gSpotLight[k].color.rgb * gSpotLight[k].intensity * attenuationfactor * falloffFactor * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        
        }
        /*
        float32_t3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
        
        halfVector = normalize(-spotLightDirectionOnSurface + toEye);
        NDotH = dot(normalize(input.normal), halfVector);
        specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
        distance = length(gSpotLight.position - input.worldPosition); //スポットライトへの距離
        float32_t attenuationfactor = pow(saturate(-distance / gSpotLight.distance + 1.0), gSpotLight.decay); //指数によるコントロール
        
        float32_t cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
        float32_t falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
        
        float32_t3 diffuseSpotLight = gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * gSpotLight.intensity * attenuationfactor * falloffFactor;
        float32_t3 specularSpotLight = gSpotLight.color.rgb * gSpotLight.intensity * attenuationfactor * falloffFactor * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        */
        
        //拡散反射+鏡面反射
        output.color.rgb = diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
        //拡散反射のみ
        //output.color.rgb = diffuseDirectionalLight + diffusePointLight + diffuseSpotLight;
        
        //アルファは今まで通り
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    
    //output.colorのα値が0のときPixelを棄却
    if (output.color.a == 0.0)
    {
        discard;
    }
    return output;
}