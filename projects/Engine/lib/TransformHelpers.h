#pragma once
#include "Vector3.h"

/// <summary>
/// Transformに関する便利関数群
/// </summary>
namespace TransformHelpers
{
	//移動方向に向く
	Vector3 FaceToVelocityDirection(const Vector3& rotate, const Vector3& velocity);

	//回転角を-πからπの範囲に正規化する
	float NormalizeAngle(float angle);

	Vector3 NormalizeAngle(const Vector3& angle);
}