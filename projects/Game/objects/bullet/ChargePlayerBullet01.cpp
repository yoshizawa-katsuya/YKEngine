#include "ChargePlayerBullet01.h"

void ChargePlayerBullet01::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle)
{
	BasePlayerBullet::Initialize(model, position, velocity, textureHandle);

	attackPower_ = 3;

	SetRadius(1.0f);
}
