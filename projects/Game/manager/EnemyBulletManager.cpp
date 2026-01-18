#include "EnemyBulletManager.h"
#include "ModelPlatform.h"
#include "CollisionManager.h"
#include "bullet/EnemyHomingBullet01.h"
#include "bullet/TargetEnemyBullet01.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void EnemyBulletManager::Initialize()
{
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	//モデル生成
	modelEnemyBulletMap_[EnemyBulletType::kTarget] = modelPlatform->CreateRigidModel("./Resources/enemyBullet", "TargetEnemyBullet.obj");
	modelEnemyBulletMap_[EnemyBulletType::kHoming] = modelPlatform->CreateRigidModel("./Resources/enemyBullet", "HomingEnemyBullet.obj");

	//調整項目をjsonに登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->CreateGroup(JsonKey::Bullet::Enemy::kGroupName);
	globalVariables->AddItem(JsonKey::Bullet::Enemy::kGroupName, JsonKey::Bullet::kRotateSpeed, 0.0f);
}

void EnemyBulletManager::Update(Camera* railCamera)
{
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<BaseEnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
		});
	// 弾更新
	for (std::unique_ptr<BaseEnemyBullet>& bullet : enemyBullets_)
	{
		bullet->Update(railCamera);
	}
}

void EnemyBulletManager::Draw(Camera* camera)
{
	for (std::unique_ptr<BaseEnemyBullet>& bullet : enemyBullets_) 
	{
		bullet->Draw(camera);
	}
}

void EnemyBulletManager::AddEnemyBullet(const Vector3& worldPosition, const Vector3& velocity, Player* target, float speed, EnemyBulletType bulletType)
{
	//ゲームオーバー時は弾を生成しない
	if (isGameOver_)
	{
		return;
	}
	
	//弾を生成し、初期化
	std::unique_ptr<BaseEnemyBullet> bullet;

	switch (bulletType)
	{
	case EnemyBulletType::kTarget:
		bullet = std::make_unique<TargetEnemyBullet01>();
		break;
	case EnemyBulletType::kHoming:
		bullet = std::make_unique<EnemyHomingBullet01>();
		break;
	default:
		break;
	}

	bullet->Initialize(modelEnemyBulletMap_[bulletType].get(), worldPosition, velocity, target, speed);

	//リストに登録する
	enemyBullets_.push_back(std::move(bullet));
}

void EnemyBulletManager::RegisterToCollisionManager(CollisionManager* collisionManager)
{
	for (std::unique_ptr<BaseEnemyBullet>& bullet : enemyBullets_) 
	{
		collisionManager->AddCollider(bullet.get());
	}
}
