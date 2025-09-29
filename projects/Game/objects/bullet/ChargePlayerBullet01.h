#pragma once
#include "BasePlayerBullet.h"

//チャージショット弾
class ChargePlayerBullet01 : public BasePlayerBullet
{
public:

	void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) override;
};

