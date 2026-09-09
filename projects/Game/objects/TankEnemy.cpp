#include "TankEnemy.h"
#include "BasePlayer.h"

using namespace YKEngine;

void TankEnemy::Initialize(const YKEngine::EnemySpawnData& spawnData, const BasePlayer* target1, const BasePlayer* target2)
{
	color_ = kColor_;

	BaseEnemy::Initialize(spawnData, target1, target2);

	
	Vector3 direction = Vector3(0.0f, 0.0f, 0.0f) - worldTransform_.GetWorldPosition();	// 中央への方向ベクトル
	velocity_ = YKEngine::Normalize(direction) * maxSpeed_;
}

void TankEnemy::Move()
{
	velocity_ = YKEngine::Normalize(velocity_) * maxSpeed_;
}
