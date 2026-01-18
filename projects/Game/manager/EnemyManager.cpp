#include "EnemyManager.h"
#include "ModelPlatform.h"
#include "ShotEnemy01.h"
#include "TackleEnemy01.h"
#include "TackleEnemy02.h"
#include "CollisionManager.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void EnemyManager::Initialize(Player* player, Camera* railCamera, EnemyBulletManager* enemyBulletManager)
{
	player_ = player;
	railCamera_ = railCamera;
	enemyBulletManager_ = enemyBulletManager;

	// 敵モデルの読み込み
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	modelEnemyMap_[EnemyType::kShot01] = modelPlatform->CreateRigidModel("./Resources/enemy", "Enemy.obj");
	modelEnemyMap_[EnemyType::kShot02] = modelPlatform->CreateRigidModel("./Resources/enemy", "Enemy02.obj");
	modelEnemyMap_[EnemyType::kTackle01] = modelPlatform->CreateRigidModel("./Resources/tackleEnemy", "TackleEnemy.obj");
	modelEnemyMap_[EnemyType::kTackle02] = modelPlatform->CreateRigidModel("./Resources/tackleEnemy", "TackleEnemy02.obj");

	//調整項目をjsonに登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& shot01GroupName = JsonKey::Enemy::Shot01::kGroupName;
	const std::string& shot02GroupName = JsonKey::Enemy::Shot02::kGroupName;

	globalVariables->CreateGroup(shot01GroupName);
	globalVariables->AddItem(shot01GroupName, JsonKey::Enemy::kFireInterval, 1.0f);
	globalVariables->AddItem(shot01GroupName, JsonKey::Enemy::kBulletSpeed, 0.5f);

	globalVariables->CreateGroup(shot02GroupName);
	globalVariables->AddItem(shot02GroupName, JsonKey::Enemy::kFireInterval, 2.0f);
	globalVariables->AddItem(shot02GroupName, JsonKey::Enemy::kBulletSpeed, 0.3f);

	// 敵のパラメータ設定
	ParamsSetup();
}

void EnemyManager::Update()
{
#ifdef _DEBUG

	// デバッグ用にパラメータ再設定
	ParamsSetup();

#endif // _DEBUG

	//デスフラグの立った敵を削除
	enemies_.remove_if([](std::unique_ptr<BaseEnemy>& enemy) {
		if (enemy->IsDisappear()) {
			return true;
		}
		return false;
		});
	// 敵の更新
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw(Camera* camera)
{
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_)
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
	case EnemyType::kShot02:
		enemy = std::make_unique<ShotEnemy01>();
		// 敵のパラメータ設定
		dynamic_cast<ShotEnemy01*>(enemy.get())->SetParams(shotEnemyParamsMap_[spawnData.type]);
		break;

	
	case EnemyType::kTackle01:
		enemy = std::make_unique<TackleEnemy01>();
		break;

	case EnemyType::kTackle02:
		enemy = std::make_unique<TackleEnemy02>();
		break;

	default:
		break;
	}
	// 敵の初期化
	enemy->Initialize(modelEnemyMap_[spawnData.type].get(), spawnData, railCamera_, player_);
	enemy->SetEnemyBulletManager(enemyBulletManager_);

	// 敵リストに追加
	enemies_.push_back(std::move(enemy));
}

void EnemyManager::RegisterToCollisionManager(CollisionManager* collisionManager)
{
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) 
	{
		if (enemy->IsDead())
		{
			continue;
		}
		collisionManager->AddCollider(enemy.get());
	}
}

void EnemyManager::ParamsSetup()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& shot01GroupName = JsonKey::Enemy::Shot01::kGroupName;
	const std::string& shot02GroupName = JsonKey::Enemy::Shot02::kGroupName;

	// 敵のパラメータ設定
	shotEnemyParamsMap_[EnemyType::kShot01].bulletType = EnemyBulletType::kTarget;
	shotEnemyParamsMap_[EnemyType::kShot01].fireInterval = globalVariables->GetFloatValue(
		shot01GroupName,
		JsonKey::Enemy::kFireInterval
	);
	shotEnemyParamsMap_[EnemyType::kShot01].bulletSpeed = globalVariables->GetFloatValue(
		shot01GroupName,
		JsonKey::Enemy::kBulletSpeed
	);

	shotEnemyParamsMap_[EnemyType::kShot02].bulletType = EnemyBulletType::kHoming;
	shotEnemyParamsMap_[EnemyType::kShot02].fireInterval = globalVariables->GetFloatValue(
		shot02GroupName,
		JsonKey::Enemy::kFireInterval
	);
	shotEnemyParamsMap_[EnemyType::kShot02].bulletSpeed = globalVariables->GetFloatValue(
		shot02GroupName,
		JsonKey::Enemy::kBulletSpeed
	);
}
