#include "EnemyBulletManager.h"
#include "ModelPlatform.h"
#include "CollisionManager.h"

using namespace YKEngine;

void EnemyBulletManager::Initialize()
{
	//テクスチャ読み込み
	textureHandleEnemyBullet_ = TextureManager::GetInstance()->Load("./Resources/red.png");

	//モデル生成
	modelBullet_ = ModelPlatform::GetInstance()->CreateSphere(textureHandleEnemyBullet_, "EnemyBullet");
}

void EnemyBulletManager::Update(Camera* railCamera)
{
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
		});
	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_)
	{
		bullet->Update(railCamera);
	}
}

void EnemyBulletManager::Draw(Camera* camera)
{
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) 
	{
		bullet->Draw(camera);
	}
}

void EnemyBulletManager::AddEnemyBullet(const Vector3& worldPosition, const Vector3& velocity, Player* target, float speed)
{
	//ゲームオーバー時は弾を生成しない
	if (isGameOver_)
	{
		return;
	}
	
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> bullet;
	bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(modelBullet_.get(), worldPosition, velocity, textureHandleEnemyBullet_, target, speed);

	//リストに登録する
	enemyBullets_.push_back(std::move(bullet));
}

void EnemyBulletManager::RegisterToCollisionManager(CollisionManager* collisionManager)
{
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) 
	{
		collisionManager->AddCollider(bullet.get());
	}
}
