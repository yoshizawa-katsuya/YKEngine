#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    //TODO: CBufferで設定できるようにする
    const float32_t2 kCenter = float32_t2(0.5f, 0.5f); //中心点
    const int32_t kNumSmaples = 10; //サンプリング数。多いほど滑らかになるが重くなる
    //TODO: CBufferで設定できるようにする
    const float32_t kBlurWidth = 0.01f; //ぼかしの幅。大きいほど強くぼかす

    //中心から現在のuvに対しての方向を計算
    //正規化せず、遠いほどより遠くをサンプリングする
    float32_t2 direction = input.texcord - kCenter;
    float32_t3 outputColor = float32_t3(0.0f, 0.0f, 0.0f);
    for (int32_t sampleIndex = 0; sampleIndex < kNumSmaples; ++sampleIndex)
    {
        //現在のuvから先ほど計算した方向にサンプリング点を進めながらサンプリングしていく
        float32_t2 texcoord = input.texcord + direction * kBlurWidth * float32_t(sampleIndex);
        
        //色を加算
        outputColor += gTexture.Sample(gSampler, texcoord).rgb;
    }
    //サンプリングした色を平均化
    outputColor.rgb *= rcp(kNumSmaples);
    
    PixelShaderOutput output;
    output.color.rgb = outputColor;
    output.color.a = 1.0f; //不透明度は1.0
    return output;
}