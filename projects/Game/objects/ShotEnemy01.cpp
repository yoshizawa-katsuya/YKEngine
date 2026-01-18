#include "ShotEnemy01.h"
#include "manager/EnemyBulletManager.h"

using namespace YKEngine;

ShotEnemy01::~ShotEnemy01()
{
}

void ShotEnemy01::MainInitialize()
{
	//発射タイマーを初期化
	fireTimer_ = params_.fireInterval + waitTime_;

}

void ShotEnemy01::UpdateMain()
{
	//発射タイマーカウントダウン
	fireTimer_ -= 1.0f / 60.0f;
	//指定時間に達した
	if (fireTimer_ <= 0.0f) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer_ = params_.fireInterval;
	}

	BaseEnemy::UpdateMain();
}

void ShotEnemy01::Fire() 
{
	//弾の速度を計算
	Vector3 velocity = Normalize(direction_);
	velocity = Multiply(params_.bulletSpeed, velocity);

	// 弾を生成し、初期化
	enemyBulletManager_->AddEnemyBullet(GetWorldPosition(), velocity, player_, params_.bulletSpeed, params_.bulletType);

}