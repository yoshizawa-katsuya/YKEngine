#pragma once
//#include "Struct.h"
#include "Lerp.h"
#include <cmath>
#include <vector>

Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t);

/*
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
	const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
*/

Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3 p2, float t);

Vector2 CatmullRom(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);

/*
void DrawCatmullRom(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
	const Vector3& controlPoint3, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
*/

Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

std::vector<Vector3> GenerateCatmullRomSplinePoints(std::vector<Vector3>& controlPoints, uint32_t numPoints);
