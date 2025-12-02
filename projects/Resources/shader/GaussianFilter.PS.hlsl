#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

static const float32_t PI = 3.14159265f;

static const float32_t2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

float gauss(float x, float y, float sigma)
{
    float exponent = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    return exp(exponent) * rcp(denominator);

}

PixelShaderOutput main(VertexShaderOutput input)
{
    //Kernelを求める。weightは後で使う
    float32_t weight = 0.0f;
    float32_t kernel3x3[3][3];
    for (int32_t x = 0; x < 3; ++x)
    {
        for (int32_t y = 0; y < 3; ++y)
        {
            kernel3x3[x][y] = gauss(kIndex3x3[x][y].x, kIndex3x3[x][y].y, 2.0f);
            weight += kernel3x3[x][y];
        }
    }
    
    uint32_t width, height; //uvStepSizeの算出
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
    PixelShaderOutput output;
    output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    for (int32_t x2 = 0; x2 < 3; ++x2) //3x3ループ
    {
        for (int32_t y2 = 0; y2 < 3; ++y2)
        {
            //現在のtexcordを算出
            float32_t2 texcord = input.texcord + kIndex3x3[x2][y2] * uvStepSize;
            //色に1/9掛けて足す
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcord).rgb;
            output.color.rgb += fetchColor * kernel3x3[x2][y2];
        }
    }
    
    //畳み込みの結果を正規化
    output.color.rgb *= rcp(weight);
    
    return output;
}