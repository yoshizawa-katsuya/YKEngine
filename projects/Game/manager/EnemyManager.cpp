#include "EnemyManager.h"
#include "ModelPlatform.h"
#include "ShotEnemy01.h"
#include "TackleEnemy01.h"
#include "CollisionManager.h"

void EnemyManager::Initialize(Player* player, Camera* railCamera, Matrix4x4* viewPortMatrix, EnemyBulletManager* enemyBulletManager)
{
	player_ = player;
	railCamera_ = railCamera;
	viewPortMatrix_ = viewPortMatrix;
	enemyBulletManager_ = enemyBulletManager;

	// 敵モデルの読み込み
	modelEnemyMap_[EnemyType::Shot01] = ModelPlatform::GetInstance()->CreateRigidModel("./Resources/enemy", "Enemy.obj");
	modelEnemyMap_[EnemyType::Tackle01] = ModelPlatform::GetInstance()->CreateRigidModel("./Resources/tackleEnemy", "TackleEnemy.obj");

}

void EnemyManager::Update()
{
	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<BaseEnemy>& enemy) {
		if (enemy->IsDead()) {
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
	// 敵の生成
	std::unique_ptr<BaseEnemy>& enemy = enemys_.emplace_back();
	// 敵の初期化
	switch (spawnData.type)
	{
	case EnemyType::Shot01:
		enemy = std::make_unique<ShotEnemy01>();

		break;

	case EnemyType::Tackle01:
		enemy = std::make_unique<TackleEnemy01>();

		break;

	default:
		break;
	}
	enemy->Initialize(modelEnemyMap_[spawnData.type].get(), spawnData, viewPortMatrix_, railCamera_);
	enemy->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	enemy->SetEnemyBulletManager(enemyBulletManager_);
}

void EnemyManager::RegisterToCollisionManager(CollisionManager* collisionManager)
{
	for (std::unique_ptr<BaseEnemy>& enemy : enemys_) 
	{
		collisionManager->AddCollider(enemy.get());
	}
}
