#pragma once
#include "Vector3.h"

namespace TransformHelpers
{
	//移動方向に向く
	Vector3 FaceToVelocityDirection(const Vector3& rotate, const Vector3& velocity);
}