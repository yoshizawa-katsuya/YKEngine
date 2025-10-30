#include "Slerp.h"
#include <algorithm>
#include <cmath>
#include "Lerp.h"
#include <numbers>

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) 
{

	//内積を求める
	float dot = Dot(v1, v2);

	//誤差により1.0fを超えるのを防ぐ
	dot = (std::min)(dot, 1.0f);

	if (dot == 1.0f)
	{
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

Vector3 SlerpTranslate(const Vector3& start, const Vector3& end, float t)
{
	const float EPS = 1e-6f;

	float lenFrom = Length(start);
	float lenTo = Length(end);

	// どちらかがほぼ原点なら線形補間にフォールバック
	if (lenFrom < EPS || lenTo < EPS) {
		return Lerp(start, end, t);
	}

	// 原点からの方向ベクトルを取得
	Vector3 a = start / lenFrom; // 正規化
	Vector3 b = end / lenTo;

	float dot = Dot(a, b);
	dot = std::clamp(dot, -1.0f, 1.0f);

	// 反対向き（dot ≈ -1）は特別処理：任意の垂直軸を選んで回転補間する
	if (std::fabs(dot + 1.0f) < 1e-5f)
	{
		// a と b は反対。回転軸として a に直交する任意のベクトルを選ぶ
		Vector3 arbitrary = { 0.0f, 1.0f, 0.0f };
		if (std::fabs(Dot(a, arbitrary)) > 0.999f) 
		{
			arbitrary = { 0.0f, 0.0f, 1.0f }; // a が up とほぼ平行なら別の軸を使う
		}

		Vector3 axis = Normalize(Cross(a, arbitrary)); // a に直交する軸
		float angle = std::numbers::pi_v<float> *t; // 0 -> 0, 1 -> pi

		// Rodrigues の回転公式（axis が単位長で a は単位長）
		Vector3 rotated = a * std::cos(angle) + Cross(axis, a) * std::sin(angle);
		float radius = Lerp(lenFrom, lenTo, t);
		return rotated * radius;
	}

	// 通常のケース
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);

	if (sinTheta < 1e-5f) {
		// 角度が小さいときはLerpで近似
		return Lerp(start, end, t);
	}

	float sinFrom = std::sin((1.0f - t) * theta);
	float sinTo = std::sin(t * theta);

	// 球面上の補間位置（方向ベクトル）
	Vector3 dir = (sinFrom * a + sinTo * b) / sinTheta;

	// 補間位置の半径を線形補間で求める
	float radius = Lerp(Length(start), Length(end), t);
	return dir * radius;
}

Vector3 SlerpTranslteByCenter(const Vector3& start, const Vector3& end, float t)
{
	// 2点の中心を求める
	Vector3 center = (start + end) * 0.5f;

	return SlerpTranslate(start - center, end - center, t) + center;
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

	if (dot == 1.0f) 
	{
		return Lerp(q1, q2, t);
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
	return (sinThetaFrom * q1 + sinThetaTo * q2) / sinTheta;

}
