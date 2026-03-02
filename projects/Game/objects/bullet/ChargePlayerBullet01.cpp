#include "ChargePlayerBullet01.h"
#include "manager/EffectManager.h"

using namespace YKEngine;

void ChargePlayerBullet01::Initialize(BaseModel* model, const Vector3& position, const Vector3& direction, BaseEnemy* targetEnemy)
{
	BasePlayerBullet::Initialize(model, position, direction, targetEnemy);

	attackPower_ = 3;

	radius_ = 1.0f;
}

void ChargePlayerBullet01::Update()
{
	BasePlayerBullet::Update();

	EffectManager::GetInstance()->SpawnEffect(EffectType::kPlayerBulletTrac02, worldTransform_.GetWorldPosition());

}
