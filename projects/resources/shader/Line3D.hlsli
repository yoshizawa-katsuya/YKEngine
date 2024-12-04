cbuffer cbuff0 : register(b0)
{
    float32_t4x4 WVP1;
    float32_t4x4 WVP2;
};

struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
};

struct GSOutput
{
    float32_t4 position : SV_POSITION;
};