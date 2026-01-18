#include "TargetEnemyBullet01.h"
#include "Player.h"

using namespace YKEngine;

void TargetEnemyBullet01::Initialize(YKEngine::BaseModel* model, const YKEngine::Vector3& position, const YKEngine::Vector3& velocity, Player* target, float speed)
{
	target_.Initialize();
	WorldTransform* targetTransform = target->GetWorldTransform();
	target_.parent_ = targetTransform->parent_;
	target_.translation_ = targetTransform->translation_;
	targetRadius_ = target->GetRadius();

	BaseEnemyBullet::Initialize(model, position, velocity, target, speed);
}

void TargetEnemyBullet01::Update(YKEngine::Camera* railCamera)
{
	target_.UpdateMatrix();

	BaseEnemyBullet::Update(railCamera);
}

void TargetEnemyBullet01::Move()
{
	if (isHoming_)
	{
		Homing();
	}

	BaseBullet::Move();
}

void TargetEnemyBullet01::Homing()
{
	//ターゲットに向かって移動する
	Vector3 direction = target_.GetWorldPosition() - worldTransform_.GetWorldPosition();
	//ゴーストと衝突したらホーミング解除
	if (Length(direction) < radius_ + targetRadius_)
	{
		isHoming_ = false;
		return;
	}
	direction = Normalize(direction);
	velocity_ = direction * speed_;
	
}