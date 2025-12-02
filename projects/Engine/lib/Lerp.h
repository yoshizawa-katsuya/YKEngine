#pragma once
#include "Struct.h"

/// <summary>
/// 線形補完。
/// </summary>
/// <param name="p0">開始点</param>
/// <param name="p1">終了点</param>
/// <param name="t">補完率(0.0~1.0)</param>
/// <returns>補完結果</returns>
float Lerp(float p0, float p1, float t);

/// <summary>
/// 線形補完。
/// </summary>
/// <param name="p0">開始点</param>
/// <param name="p1">終了点</param>
/// <param name="t">補完率(0.0~1.0)</param>
/// <returns>補完結果</returns>
Vector2 Lerp(Vector2 p0, Vector2 p1, float t);

/// <summary>
/// 線形補完。
/// </summary>
/// <param name="p0">開始点</param>
/// <param name="p1">終了点</param>
/// <param name="t">補完率(0.0~1.0)</param>
/// <returns>補完結果</returns>
Vector3 Lerp(const Vector3& p0, const Vector3& p1, float t);

/// <summary>
/// 線形補完。
/// </summary>
/// <param name="p0">開始点</param>
/// <param name="p1">終了点</param>
/// <param name="t">補完率(0.0~1.0)</param>
/// <returns>補完結果</returns>
Vector4 Lerp(const Vector4& p0, const Vector4& p1, float t);

/// <summary>
/// 角度の線形補完。
/// </summary>
/// <param name="a0">開始角度</param>
/// <param name="a1">終了角度</param>
/// <param name="t">補完率(0.0~1.0)</param>
/// <returns>補完結果</returns>
float LerpAngle(float a0, float a1, float t);

/// <summary>
/// 角度の線形補完。
/// </summary>
/// <param name="a0">開始角度</param>
/// <param name="a1">終了角度</param>
/// <param name="t">補完率(0.0~1.0)</param>
/// <returns>補完結果</returns>
Vector3 LerpAngle(const Vector3& a0, const Vector3& a1, float t);

/// <summary>
/// 四元数の線形補完。
/// </summary>
/// <param name="p0">開始点</param>
/// <param name="p1">終了点</param>
/// <param name="t">補完率(0.0~1.0)</param>
/// <returns>補完結果</returns>
Quaternion Lerp(const Quaternion& p0, const Quaternion& p1, float t);

/// <summary>
/// 最短角度への線形補完。
/// </summary>
/// <param name="a">開始角度</param>
/// <param name="b">終了角度</param>
/// <param name="t">補完率(0.0~1.0)</param>
/// <returns>補完結果</returns>
float LeapShortAngle(float a, float b, float t);
