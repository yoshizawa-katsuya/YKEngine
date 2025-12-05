#pragma once
#include "BasePlayerBullet.h"

/// <summary>
/// チャージショット弾。
/// 通常の弾よりも大きく、威力が高い。
/// プレイヤーがチャージショットを撃つときに生成される。
/// </summary>
class ChargePlayerBullet01 : public BasePlayerBullet
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期位置</param>
	/// <param name="velocity">初速度</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(YKEngine::BaseModel* model, const YKEngine::Vector3& position, const YKEngine::Vector3& velocity, uint32_t textureHandle) override;
};

