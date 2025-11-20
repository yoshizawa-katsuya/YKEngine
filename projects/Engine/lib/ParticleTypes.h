#pragma once
#include <numbers>
#include "Easing.h"

/// <summary>
/// パーティクルのランダム化フラグ。
/// 各プロパティに対してランダム化を行うかどうかを指定する。
/// </summary>
struct ParticleRandomizationFlags
{
	bool color = false;
	bool translate = false;
	bool velocity = false;
	bool rotate = false;
	bool scale = false;
	bool lifeTime = false;
};

/// <summary>
/// 2つの値の範囲を表す構造体です。
/// </summary>
/// <typeparam name="T">範囲の値の型。</typeparam>
template<typename T>
struct RandomRange
{
	T max;
	T min;
};

/// <summary>
/// ランダム化するパーティクルの各種パラメータの範囲を定義する構造体。
/// 各パラメータはRandomRange構造体を使用して、最大値と最小値の範囲を指定します。
/// </summary>
struct EmitterRangeParams
{
	EmitterRangeParams()
		: translate({ Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ -1.0f, -1.0f, -1.0f } })
		, scale ({ { 1.0f, 1.0f, 1.0f } , { -1.0f, -1.0f, -1.0f } })
		, rotate ({ { std::numbers::pi_v<float>, std::numbers::pi_v<float>, std::numbers::pi_v<float> } ,{ -std::numbers::pi_v<float>, -std::numbers::pi_v<float>, -std::numbers::pi_v<float> } })
		, velocity({ Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ -1.0f, -1.0f, -1.0f } })
		, lifeTime({3.0f, 1.0f})
	{
	}
	RandomRange<Vector3> translate;
	RandomRange<Vector3> scale;
	RandomRange<Vector3> rotate;
	RandomRange<Vector3> velocity;
	RandomRange<float> lifeTime;
};

/// <summary>
/// パーティクルのブレンドモード
/// </summary>
enum class ParticleBlendMode
{
	kAddBlend,
	kNormalBlend,
};

/// <summary>
/// パーティクルの挙動を定義する構造体。
/// </summary>
/// <param name="isFaceToVelocityDirection">進行方向を向くかどうか。</param>
/// <param name="isConstantVelocity">速さを一定にするかどうか。</param>
/// <param name="speed">一定速度にする場合の速さ。</param>
/// <param name="isScaleToDisappear">少しずつScaleを小さくするかどうか。</param>
/// <param name="isScaleToAppear">少しずつScaleを大きくするかどうか。</param>
/// <param name="easingTypeForScale">Scaleの変化に使用するイージングタイプ。</param>
/// <param name="isUseBillboard">ビルボードを使用するかどうか。</param>
/// <param name="isdownVelocity">少しずつ速度を下げるかどうか。</param>
struct ParticleBehavior
{
	//進行方向を向くかどうか
	bool isFaceToVelocityDirection = false;
	//速さを一定にするかどうか
	bool isConstantVelocity = false;
	float speed = 1.0f;
	//少しずつScaleを小さくするかどうか
	bool isScaleToDisappear = false;
	//少しずつScaleを大きくするかどうか
	bool isScaleToAppear = false;
	EasingType easingTypeForScale = EasingType::kLinear;
	//ビルボードを使用するかどうか
	bool isUseBillboard = true;
	//少しずつ速度を下げるかどうか
	bool isdownVelocity = false;
	//パーティクルの初期位置をemitterから一定の距離に固定するか
	bool isfixedDistance = false;
	float distance = 1.0f;
	//パーティクルがエミッターの中心地点に向かうならtrue
	bool isHeadCenter = false;
	//時間経過で透明になっていくならtrue
	bool isTimeFadeOut = true;
	//ブレンドモード
	ParticleBlendMode blendMode = ParticleBlendMode::kAddBlend;
};