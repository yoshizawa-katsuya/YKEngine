#include "object3d.hlsli"

struct TransformationMatrix
{
    float32_t4x4 WVP;
    float32_t4x4 World;
    float32_t4x4 WorldInverseTranspose;
};
StructuredBuffer<TransformationMatrix> gTransformationMatrix : register(t0);



struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrix[instanceId].WVP);
    output.texcord = input.texcord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gTransformationMatrix[instanceId].WorldInverseTranspose));
    output.worldPosition = mul(input.position, gTransformationMatrix[instanceId].World).xyz;
    return output;
}