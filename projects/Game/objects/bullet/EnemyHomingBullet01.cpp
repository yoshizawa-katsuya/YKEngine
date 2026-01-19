#include "EnemyHomingBullet01.h"
#include "Player.h"
#include "Lerp.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void EnemyHomingBullet01::Initialize(YKEngine::BaseModel* model, const YKEngine::Vector3& position, const YKEngine::Vector3& velocity, Player* target, float speed)
{
	target_ = target;

	BaseEnemyBullet::Initialize(model, position, velocity, target, speed);
}

void EnemyHomingBullet01::Move()
{
	if (isHoming_)
	{
		Homing();
	}

	//通常の移動処理
	BaseBullet::Rotate();
	BaseBullet::Move();
}

void EnemyHomingBullet01::Homing()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Bullet::Enemy::kGroupName;

	//ターゲットに向かって移動する
	Vector3 direction = target_->GetWorldPosition() - worldTransform_.GetWorldPosition();
	//ある程度近づいたらホーミングをやめる
	if (Length(direction) < globalVariables->GetFloatValue(groupName, JsonKey::Bullet::kStopDistance))
	{
		isHoming_ = false;
		return;
	}
	direction = Normalize(direction);
	Vector3 homing = direction * speed_;

	//補間を使ってなめらかにする
	velocity_ = Lerp(velocity_, homing, globalVariables->GetFloatValue(groupName, JsonKey::Bullet::kHomingLerpFactor));
}
