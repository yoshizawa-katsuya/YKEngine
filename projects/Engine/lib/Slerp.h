#pragma once
#include "Vector3.h"
#include "Quaternion.h"

//球面線形補完
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

Quaternion Slerp(const Quaternion& v1, const Quaternion& v2, float t);
