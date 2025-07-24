#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t> gMaskTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    float32_t mask = gMaskTexture.Sample(gSampler, input.texcord);
    //maskの値が0.5(閾値)以下の場合はdiscardして抜く
    //TODO : 閾値をCBufferで設定できるようにする
    float32_t threshold = 0.2f; // 閾値を定義
    if (mask <= threshold) 
    {
        discard;
    }
    
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcord);
    
    //Edgeっぽさを検出
    float32_t edge = 1.0f - smoothstep(threshold, threshold + 0.03f, mask);
    //Edgeっぽいほど指定した色を加算
    output.color.rgb += edge * float32_t3(1.0f, 0.4f, 0.3f); // 赤っぽい色を加算
    
    return output;
}