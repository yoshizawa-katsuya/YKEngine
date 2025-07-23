#include "FullScreen.hlsli"

struct Material
{
    float32_t4x4 projectionInverse;
    
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
Texture2D<float32_t> gDepthTexture : register(t1);
SamplerState gSamplerPoint : register(s1);

static const float32_t2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float32_t kPrewittHorizontalKernel[3][3] = {
    {-1.0f / 6.0f, 0.0f, 1.0f / 6.0f},
    {-1.0f / 6.0f, 0.0f, 1.0f / 6.0f},
    {-1.0f / 6.0f, 0.0f, 1.0f / 6.0f }
};

static const float32_t kPrewittVerticalKernel[3][3] = {
    {-1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f},
    {0.0f, 0.0f, 0.0f},
    {1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f}
};

float32_t Luminance(float32_t3 v)
{
    return dot(v, float32_t3(0.2125f, 0.7154f, 0.0721f));
}

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    
    uint32_t width, height; //uvStepSizeの算出
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));   
    
    float32_t2 difference = float32_t2(0.0f, 0.0f); //縦横それぞれの畳み込みの結果を格納する変数
    //色を輝度に変換して、畳み込みを行っていく
    for (int32_t x = 0; x < 3; ++x)
    {
        for (int32_t y = 0; y < 3; ++y)
        {
            float32_t2 texcord = input.texcord + kIndex3x3[x][y] * uvStepSize;
            float32_t ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcord); //NDC座標の深度値を取得
            float32_t4 viewSpace = mul(float32_t4(0.0f, 0.0f, ndcDepth, 1.0f), gMaterial.projectionInverse);
            float32_t viewZ = viewSpace.z * rcp(viewSpace.w); //同時座標系からデカルト座標系に変換
            difference.x += viewZ * kPrewittHorizontalKernel[x][y]; //水平方向の畳み込み
            difference.y += viewZ * kPrewittVerticalKernel[x][y]; //垂直方向の畳み込み
        }
    }

    //変化の長さをウェイトとして合成
    float32_t weight = length(difference);
    //ウェイトを0.0fから1.0fの範囲に収める
    weight = saturate(weight);
    
    PixelShaderOutput output;
    //weightが大きいほど暗くなるように設定
    output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler, input.texcord).rgb;
    output.color.a = 1.0f; //アルファ値は1.0fに設定
    
    return output;
}