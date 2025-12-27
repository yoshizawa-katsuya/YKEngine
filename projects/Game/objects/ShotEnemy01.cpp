#include "ShotEnemy01.h"
#include "manager/EnemyBulletManager.h"

using namespace YKEngine;

ShotEnemy01::~ShotEnemy01()
{
}

void ShotEnemy01::MainInitialize()
{
	//発射タイマーを初期化
	fireTimer_ = kFireInterval_;

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
		fireTimer_ = kFireInterval_;
	}

	BaseEnemy::UpdateMain();
}

void ShotEnemy01::Fire() 
{

	//弾の速さ
	const float kBulletSpeed = 0.5f;

	Vector3 velocity = Normalize(direction_);
	velocity = Multiply(kBulletSpeed, velocity);

	// 弾を生成し、初期化
	enemyBulletManager_->AddEnemyBullet(GetWorldPosition(), velocity, player_, kBulletSpeed, bulleyType_);

}