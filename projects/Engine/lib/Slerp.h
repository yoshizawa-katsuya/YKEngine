#pragma once
#include "Vector3.h"
#include "Quaternion.h"

//球面線形補完
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

//球面線形補完(位置ベクトル版)
Vector3 SlerpTranslate(const Vector3& start, const Vector3& end, float t);

//球面線形補完(座標の中心を基準にした位置ベクトル版)
Vector3 SlerpTranslteByCenter(const Vector3& start, const Vector3& end, float t);

//球面線形補完(座標の中心を基準にした位置ベクトル版、中心座標指定可能)
Vector3 SlerpTranslteByCenter(const Vector3& center, const Vector3& start, const Vector3& end, float t);

//球面線形補完(クォータニオン版)
Quaternion Slerp(const Quaternion& v1, const Quaternion& v2, float t);
