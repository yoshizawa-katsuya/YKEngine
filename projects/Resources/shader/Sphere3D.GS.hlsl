#include "Sphere3D.hlsli"

//円周率
static const float pi = 3.1415927f;

//球の分割数
static const uint kSubdivision = 8;

//経度分割1つ分の角度 φ
static const float kLonEvery = pi * 2.0f / float(kSubdivision);
//緯度分割1つ分の角度 θ
static const float kLatEvery = pi / float(kSubdivision);

[maxvertexcount(kSubdivision * kSubdivision * 4)]
void main(
	point VertexShaderOutput input[1],
	inout LineStream<GSOutput> output
)
{
	
    GSOutput element;
    float32_t4 a, b, c;
    
    
	//緯度の方向に分割
    for (uint latIndex = 0; latIndex < kSubdivision; ++latIndex)
    {
        float lat = -pi / 2.0f + kLatEvery * latIndex; //θ
		//経度の方向に分割しながら線を描く
        for (uint lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
        {
            
            float lon = lonIndex * kLonEvery; //φ
			//頂点にデータを入力する。基準点a
            a.x = cos(lat) * cos(lon) * 0.5f;
            a.y = sin(lat) * 0.5f;
            a.z = cos(lat) * sin(lon) * 0.5f;
            a.w = 1.0f;
            a = input[0].position + a;
            a = mul(a, WVP);
            
			//b
            b.x = cos(lat + kLatEvery) * cos(lon) * 0.5f;
            b.y = sin(lat + kLatEvery) * 0.5f;
            b.z = cos(lat + kLatEvery) * sin(lon) * 0.5f;
            b.w = 1.0f;
            b = input[0].position + b;
            b = mul(b, WVP);
            
			//c
            c.x = cos(lat) * cos(lon + kLonEvery) * 0.5f;
            c.y = sin(lat) * 0.5f;
            c.z = cos(lat) * sin(lon + kLonEvery) * 0.5f;
            c.w = 1.0f;
            c = input[0].position + c;
            c = mul(c, WVP);
			
            element.position = a;
            output.Append(element);
            
            element.position = b;
            output.Append(element);
            
            element.position = c;
            output.Append(element);
            
            element.position = a;
            output.Append(element);
            
            //現在のストリップを終了
            output.RestartStrip();
        }
    }

}