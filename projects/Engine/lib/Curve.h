#pragma once
#include "Lerp.h"
#include <cmath>
#include <vector>

namespace YKEngine
{

/// <summary>
/// ベジェ曲線の点を計算する。
/// </summary>
/// <param name="p0">制御点0。</param>
/// <param name="p1">制御点1。</param>
/// <param name="p2">制御点2。</param>
/// <param name="t">パラメータ（0.0〜1.0）。</param>
/// <returns>計算された点。</returns>
Vector2 Bezier(Vector2 p0, Vector2 p1, Vector2 p2, float t);

/// <summary>
/// ベジェ曲線の点を計算する。
/// </summary>
/// <param name="p0">制御点0。</param>
/// <param name="p1">制御点1。</param>
/// <param name="p2">制御点2。</param>
/// <param name="t">パラメータ（0.0〜1.0）。</param>
/// <returns>計算された点。</returns>
Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3 p2, float t);

/// <summary>
/// Catmull-Romスプラインの点を計算する。
/// </summary>
/// <param name="p0">制御点0。</param>
/// <param name="p1">制御点1。</param>
/// <param name="p2">制御点2。</param>
/// <param name="p3">制御点3。</param>
/// <param name="t">パラメータ（0.0〜1.0）。</param>
/// <returns>計算された点。</returns>
Vector2 CatmullRom(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t);

/// <summary>
/// Catmull-Romスプラインの点を計算する。
/// </summary>
/// <param name="p0">制御点0。</param>
/// <param name="p1">制御点1。</param>
/// <param name="p2">制御点2。</param>
/// <param name="p3">制御点3。</param>
/// <param name="t">パラメータ（0.0〜1.0）。</param>
/// <returns>計算された点。</returns>
Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

/// <summary>
/// Catmull-Romスプラインの点群を生成する。
/// </summary>
/// <param name="controlPoints">制御点群。</param>
/// <param name="numPoints">生成する点の数。</param>
/// <returns>生成された点群。</returns>
std::vector<Vector3> GenerateCatmullRomSplinePoints(const std::vector<Vector3>& controlPoints, uint32_t numPoints);

/// <summary>
/// Catmull-Romスプラインの点群をループさせて生成する。
/// </summary>
/// <param name="controlPoints">制御点群。</param>
/// <param name="numPoints">生成する点の数。</param>
/// <returns>生成された点群。</returns>
std::vector<Vector3> GenerateCatmullRomSplinePointsLoop(const std::vector<Vector3>& controlPoints, uint32_t numPoints);

} // namespace YKEngine