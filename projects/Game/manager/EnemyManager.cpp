#include "EnemyManager.h"
#include "ModelPlatform.h"
#include "ShotEnemy01.h"
#include "ShotEnemy02.h"
#include "TackleEnemy01.h"
#include "CollisionManager.h"

using namespace YKEngine;

void EnemyManager::Initialize(Player* player, Camera* railCamera, EnemyBulletManager* enemyBulletManager)
{
	player_ = player;
	railCamera_ = railCamera;
	enemyBulletManager_ = enemyBulletManager;

	// 敵モデルの読み込み
	modelEnemyMap_[EnemyType::kShot01] = ModelPlatform::GetInstance()->CreateRigidModel("./Resources/enemy", "Enemy.obj");
	modelEnemyMap_[EnemyType::kShot02] = ModelPlatform::GetInstance()->CreateRigidModel("./Resources/enemy", "Enemy02.obj");
	modelEnemyMap_[EnemyType::kTackle01] = ModelPlatform::GetInstance()->CreateRigidModel("./Resources/tackleEnemy", "TackleEnemy.obj");

}

void EnemyManager::Update()
{
	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<BaseEnemy>& enemy) {
		if (enemy->IsDisappear()) {
			return true;
		}
		return false;
		});
	// 敵の更新
	for (std::unique_ptr<BaseEnemy>& enemy : enemys_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw(Camera* camera)
{
	for (std::unique_ptr<BaseEnemy>& enemy : enemys_)
	{
		enemy->Draw(camera);
	}
}

void EnemyManager::PopEnemy(const EnemySpawn& spawnData)
{
	std::unique_ptr<BaseEnemy> enemy;

	switch (spawnData.type)
	{
	case EnemyType::kShot01:
		enemy = std::make_unique<ShotEnemy01>();
		break;

	case EnemyType::kShot02:
		enemy = std::make_unique<ShotEnemy02>();
		break;

	case EnemyType::kTackle01:
		enemy = std::make_unique<TackleEnemy01>();
		break;

	default:
		break;
	}
	// 敵の初期化
	enemy->Initialize(modelEnemyMap_[spawnData.type].get(), spawnData, railCamera_, player_);
	enemy->SetEnemyBulletManager(enemyBulletManager_);

	// 敵リストに追加
	enemys_.push_back(std::move(enemy));
}

void EnemyManager::RegisterToCollisionManager(CollisionManager* collisionManager)
{
	for (std::unique_ptr<BaseEnemy>& enemy : enemys_) 
	{
		if (enemy->IsDead())
		{
			continue;
		}
		collisionManager->AddCollider(enemy.get());
	}
}
