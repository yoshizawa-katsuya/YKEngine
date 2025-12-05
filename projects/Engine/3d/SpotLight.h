#pragma once
#include <Vector3.h>
#include <Vector4.h>
#include <numbers>
#include <cmath>

namespace YKEngine
{

/// <summary>
/// スポットライト。
/// 使用する場合、modelPtalformへデータをセットすること。
/// </summary>
/// <param name="color">ライトの色</param>
/// <param name="position">ライトの位置</param>
/// <param name="intensity">輝度</param>
/// <param name="direction">ライトの向き</param>
/// <param name="distance">ライトの届く最大距離</param>
/// <param name="decay">減衰率</param>
/// <param name="cosAngle">スポットライトの余弦</param>
/// <param name="cosFalloffStart">falloff開始の角度</param>
struct SpotLight
{
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };	//ライトの色
	Vector3 position = { 2.0f, 1.25f, 0.0f };	//ライトの位置
	float intensity = 1.0f;	//輝度
	Vector3 direction = Normalize(Vector3{ -1.0f, -1.0f, 0.0f });	//ライトの向き
	float distance = 7.0f;	//ライトの届く最大距離
	float decay = 2.0f;	//減衰率
	float cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);	//スポットライトの余弦
	float cosFalloffStart = 1.0f;	//falloff開始の角度
	float padding;
};

} // namespace YKEngine