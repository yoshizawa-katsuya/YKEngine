#pragma once
#include <Vector3.h>
#include <Vector4.h>

namespace YKEngine
{

/// <summary>
/// 点光源。
/// 使用する場合、modelPtalformへデータをセットすること。
/// </summary>
/// <param name="color">ライトの色</param>
/// <param name="position">ライトの位置</param>
/// <param name="intensity">輝度</param>
/// <param name="radius">ライトの届く最大距離</param>
/// <param name="decay">減衰率</param>
struct PointLight
{
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };	//ライトの色
	Vector3 position{};
	float intensity = 1.0f;	//輝度
	float radius = 3.0f;	//ライトの届く最大距離
	float decay = 1.0f;	//減衰率
	float padding[2];
};

} // namespace YKEngine