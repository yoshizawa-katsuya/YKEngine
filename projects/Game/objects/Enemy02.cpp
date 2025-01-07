#include "Enemy02.h"
#include "GameScene.h"

Enemy02::Enemy02()
{
}

Enemy02::~Enemy02()
{
}

void Enemy02::Initialize(BaseModel* model, const EulerTransform& transform)
{
	BaseEnemy::Initialize(model, transform);

	speed_ = 0.03f;
}

void Enemy02::Update()
{
	interval_--;

	if (interval_ <= 0) {
		interval_ = 60;
		gameScene_->AddEnemyBullet({ worldTransform_.translation_.x,  worldTransform_.translation_.y - 1.0f ,worldTransform_.translation_.z }, { 0.0f, -bulletSpeed_, 0.0f });
		gameScene_->AddEnemyBullet({ worldTransform_.translation_.x,  worldTransform_.translation_.y + 1.0f ,worldTransform_.translation_.z }, { 0.0f, bulletSpeed_, 0.0f });
	}

	BaseEnemy::Update();

}
