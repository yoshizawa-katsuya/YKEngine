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

	void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) override;
};

