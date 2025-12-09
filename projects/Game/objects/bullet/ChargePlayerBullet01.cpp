#include "ChargePlayerBullet01.h"

using namespace YKEngine;

void ChargePlayerBullet01::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity)
{
	BasePlayerBullet::Initialize(model, position, velocity);

	attackPower_ = 3;

	SetRadius(1.0f);
}
