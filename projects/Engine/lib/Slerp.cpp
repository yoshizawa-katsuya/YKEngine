#include "Slerp.h"
#include <algorithm>
#include <cmath>
#include "Lerp.h"

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {

	//内積を求める
	float dot = Dot(v1, v2);

	//誤差により1.0fを超えるのを防ぐ
	dot = (std::min)(dot, 1.0f);

	if (dot == 1.0f) {
		return Lerp(v1, v2, t);
	}

	//アークコサインでθの角度を求める
	float theta = std::acos(dot);
	//θの角度からsinθを求める
	float sinTheta = std::sin(theta);
	//sin(θ(1-t))を求める
	float sinThetaFrom = std::sin((1 - t) * theta);
	//sinθtを求める
	float sinThetaTo = std::sin(t * theta);


	//球面線形補完したベクトル
	return (sinThetaFrom * v1 + sinThetaTo * v2) / sinTheta;

}

Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{

	if (q1 == q2) {
		return q1;
	}

	//内積を求める
	float dot = Dot(q1, q2);

	//誤差により1.0fを超えるのを防ぐ
	dot = (std::min)(dot, 1.0f);

	if (dot == 1.0f) {
		return Lerp(q1, q2, t);
	}
	/*
	else if (dot < 0.0f)
	{
		q1 = -q1;
		dot = -dot;
	}
	*/
	//アークコサインでθの角度を求める
	float theta = std::acos(dot);
	//θの角度からsinθを求める
	float sinTheta = std::sin(theta);
	//sin(θ(1-t))を求める
	float sinThetaFrom = std::sin((1 - t) * theta);
	//sinθtを求める
	float sinThetaTo = std::sin(t * theta);

	//球面線形補完したベクトル
	return (sinThetaFrom * q1 + sinThetaTo * q2) / sinTheta;

}
