#pragma once
#include <Vector3.h>
#include <Vector4.h>

namespace YKEngine
{

/// <summary>
/// 平行光源。
/// 使用する場合、modelPtalformへデータをセットすること。
/// </summary>
///	<param name="color">ライトの色</param>
/// <param name="direction">ライトの向き</param>
/// <param name="intensity">輝度</param>
struct DirectionalLight
{
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };	//ライトの色
	Vector3 direction = { -0.3f, -1.0f, -0.3f };	//ライトの向き
	float intensity = 1.0f;	//輝度
};

} // namespace YKEngine