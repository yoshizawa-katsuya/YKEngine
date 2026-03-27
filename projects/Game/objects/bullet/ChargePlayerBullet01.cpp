#include "ChargePlayerBullet01.h"
#include "manager/EffectManager.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void ChargePlayerBullet01::Initialize(BaseModel* model, const Vector3& position, const Vector3& direction, BaseEnemy* targetEnemy)
{
	BasePlayerBullet::Initialize(model, position, direction, targetEnemy);

	attackPower_ = GlobalVariables::GetInstance()->GetIntValue(JsonKey::Bullet::Player::Charge01::kGroupName, JsonKey::Bullet::kAttackPower);

	SetRadius(GlobalVariables::GetInstance()->GetFloatValue(JsonKey::Bullet::Player::Charge01::kGroupName, JsonKey::Bullet::kRadius));
}

void ChargePlayerBullet01::Update()
{
	BasePlayerBullet::Update();

	EffectManager::GetInstance()->SpawnEffect(EffectType::kPlayerBulletTrac02, worldTransform_.GetWorldPosition());

}
