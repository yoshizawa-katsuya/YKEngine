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
	//モデル生成に使用するModelPlatformクラスのインスタンスを取得
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	//モデル生成
	modelEnemyBulletMap_[EnemyBulletType::kTarget] = modelPlatform->CreateRigidModel("./Resources/enemyBullet", "TargetEnemyBullet.obj");
	modelEnemyBulletMap_[EnemyBulletType::kHoming] = modelPlatform->CreateRigidModel("./Resources/enemyBullet", "HomingEnemyBullet.obj");

	//調整項目をjsonに登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& enemyBulletGroupName = JsonKey::Bullet::Enemy::kGroupName;
	globalVariables->CreateGroup(enemyBulletGroupName);
	globalVariables->AddItem(enemyBulletGroupName, JsonKey::Bullet::kRotateSpeed, 0.0f);
	globalVariables->AddItem(enemyBulletGroupName, JsonKey::Bullet::kStopDistance, 5.0f);
	globalVariables->AddItem(enemyBulletGroupName, JsonKey::Bullet::kHomingLerpFactor, 0.13f);
}

void EnemyBulletManager::Update(Camera* railCamera)
{
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<BaseEnemyBullet>& bullet) {
		if (bullet->IsDead()) 
		{
			CollisionManager::GetInstance()->RemoveSphereCollider(bullet.get());
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
	//bulletTypeに対応する弾の生成関数を取得
	const std::unordered_map<EnemyBulletType, EnemyBulletFactory>& enemyBulletFactoryMap = GetEnemyBulletFactoryMap();

	//検索
	auto it = enemyBulletFactoryMap.find(bulletType);
	if (it == enemyBulletFactoryMap.end())
	{
		assert(false && "Unknown EnemyBulletType"); // 弾の種類が見つからない場合はエラー
		return;
	}

	//弾を生成し、初期化
	std::unique_ptr<BaseEnemyBullet> bullet = it->second();

	bullet->Initialize(modelEnemyBulletMap_[bulletType].get(), worldPosition, velocity, target, speed);

	//リストに登録する
	enemyBullets_.push_back(std::move(bullet));

	//生成した弾のコライダーを衝突管理クラスに登録する
	CollisionManager::GetInstance()->AddSphereCollider(enemyBullets_.back().get());
}

const std::unordered_map<EnemyBulletType, EnemyBulletManager::EnemyBulletFactory>& EnemyBulletManager::GetEnemyBulletFactoryMap() const
{
	static const std::unordered_map<EnemyBulletType, EnemyBulletFactory> enemyBulletFactoryMap =
	{
		{EnemyBulletType::kTarget, []() { return std::make_unique<TargetEnemyBullet01>(); }},
		{EnemyBulletType::kHoming, []() { return std::make_unique<EnemyHomingBullet01>(); }},
	};
	return enemyBulletFactoryMap;
}
