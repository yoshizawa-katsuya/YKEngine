#include "Sprite.hlsli"

//TODO : Material構造体の内容を適切に変更する
struct Material
{
    float32_t4 color;
    int32_t enableLighting;

    float shininess;
    float enviromentCoefficient;
    float padding;

    float32_t4x4 uvTransform;

    float progress;
    float3 padding2;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gMaskTexture : register(t1);
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
    float mask = gMaskTexture.Sample(gSampler, transformedUV.xy).r;

    if (mask < gMaterial.progress)
    {
        discard;
    }
    
    output.color = gMaterial.color * textureColor;
    
    if (output.color.a == 0.0)
    {
        discard;
    }
    return output;
}