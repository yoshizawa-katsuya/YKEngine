#include "Line3D.hlsli"

[maxvertexcount(2)]
void main(
	point VertexShaderOutput input[1], 
	inout LineStream<GSOutput> output
)
{
	
    GSOutput element;
	
    element.position = mul(input[0].position, WVP1);
    output.Append(element);
	
    element.position = mul(input[0].position, WVP2);
    output.Append(element);

}