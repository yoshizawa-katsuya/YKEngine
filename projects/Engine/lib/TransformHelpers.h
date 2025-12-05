#pragma once
#include "Vector3.h"

namespace YKEngine
{

/// <summary>
/// Transformに関する便利関数群
/// </summary>
namespace TransformHelpers
{
	/// <summary>
	/// 回転角rotateに対して、velocityの方向を向くような回転角を計算する。
	/// </summary>
	/// <param name="rotate">基準となる回転角</param>
	/// <param name="velocity">基準となる速度ベクトル</param>
	/// <returns>velocityの方向を向く回転角</returns>
	Vector3 FaceToVelocityDirection(const Vector3& rotate, const Vector3& velocity);

	/// <summary>
	/// 角度を-180～180度の範囲に正規化する。
	/// </summary>
	/// <param name="angle">正規化する角度</param>
	/// <returns>正規化された角度</returns>
	float NormalizeAngle(float angle);

	/// <summary>
	/// 角度を-180～180度の範囲に正規化する。
	/// </summary>
	/// <param name="angle">正規化する角度</param>
	/// <returns>正規化された角度</returns>
	Vector3 NormalizeAngle(const Vector3& angle);
}

} // namespace YKEngine