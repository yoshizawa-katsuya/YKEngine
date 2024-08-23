#include "Slerp.h"
#include "Vector.h"
#include <algorithm>
#include <cmath>
#include "Lerp.h"
#include "Quaternion.h"

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) { 
	
	Vector3 v1N = Normalize(v1);
	Vector3 v2N = Normalize(v2);

	//内積を求める
	float dot = Dot(v1N, v2N);

	//誤差により1.0fを超えるのを防ぐ
	dot = (std::min)(dot, 1.0f);
	//アークコサインでθの角度を求める
	float theta = std::acos(dot);
	//θの角度からsinθを求める
	float sinTheta = std::sin(theta);
	//sin(θ(1-t))を求める
	float sinThetaFrom = std::sin((1 - t) * theta);
	//sinθtを求める
	float sinThetaTo = std::sin(t * theta);


	Vector3 normalizationCompletion;
	//ゼロ除算を防ぐ
	if (sinTheta < 1.0e-5) {
		//normalizationCompletion = v1N;
		sinTheta = 0.001f;
		normalizationCompletion = Normalize((sinThetaFrom * v1N + sinThetaTo * v2N) / sinTheta);
	} else {
		//球面線形補完したベクトル
		normalizationCompletion = (sinThetaFrom * v1N + sinThetaTo * v2N) / sinTheta;
	}

	//ベクトルの長さはv1とv2の長さを線形補間
	float length1 = Length(v1);
	float length2 = Length(v2);

	//Lerpで補間ベクトルの長さを求める
	float length = Lerp(length1, length2, t);

	//長さを反映
	return length * normalizationCompletion;

}

Quaternion Slerp(const Quaternion& v1, const Quaternion& v2, float t)
{
	
	Quaternion v1N = Normalize(v1);
	Quaternion v2N = Normalize(v2);

	//内積を求める
	float dot = Dot(v1N, v2N);

	//誤差により1.0fを超えるのを防ぐ
	dot = (std::min)(dot, 1.0f);
	//アークコサインでθの角度を求める
	float theta = std::acos(dot);
	//θの角度からsinθを求める
	float sinTheta = std::sin(theta);
	//sin(θ(1-t))を求める
	float sinThetaFrom = std::sin((1 - t) * theta);
	//sinθtを求める
	float sinThetaTo = std::sin(t * theta);


	Quaternion normalizationCompletion;
	//ゼロ除算を防ぐ
	if (sinTheta < 1.0e-5) {
		//normalizationCompletion = v1N;
		sinTheta = 0.001f;
		normalizationCompletion = Normalize((sinThetaFrom * v1N + sinThetaTo * v2N) / sinTheta);
	}
	else {
		//球面線形補完したベクトル
		normalizationCompletion = (sinThetaFrom * v1N + sinThetaTo * v2N) / sinTheta;
	}

	//ベクトルの長さはv1とv2の長さを線形補間
	float length1 = Length(v1);
	float length2 = Length(v2);

	//Lerpで補間ベクトルの長さを求める
	float length = Lerp(length1, length2, t);

	//長さを反映
	return length * normalizationCompletion;

}
