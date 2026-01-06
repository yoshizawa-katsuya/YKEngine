#include "PlayerBulletManager.h"
#include "ModelPlatform.h"
#include "CollisionManager.h"
#include "bullet/PlayerBullet01.h"
#include "bullet/ChargePlayerBullet01.h"

using namespace YKEngine;

void PlayerBulletManager::Initialize()
{

	//モデル生成
	modelBullet_ = ModelPlatform::GetInstance()->CreateRigidModel("./Resources/playerBullet", "PlayerBullet.obj");
}

void PlayerBulletManager::Update()
{
	// デスフラグの立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<BasePlayerBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
		});
	//弾更新
	for (std::unique_ptr<BasePlayerBullet>& bullet : playerBullets_) 
	{
		bullet->Update();
	}
}

void PlayerBulletManager::Draw(Camera* camera)
{
	//弾描画
	for (std::unique_ptr<BasePlayerBullet>& bullet : playerBullets_) 
	{
		bullet->Draw(camera);
	}
}

void PlayerBulletManager::AddPlayerBullet(const Vector3& worldPosition, const Vector3& direction, PlayerBulletType bulletType, BaseEnemy* targetEnemy)
{
	//リストに登録する
	//弾を生成し、初期化
	std::unique_ptr<BasePlayerBullet> bullet;
	switch (bulletType)
	{
	case PlayerBulletType::kNormal:
		bullet = std::make_unique<PlayerBullet01>();

		break;
	case PlayerBulletType::kCharge:
		bullet = std::make_unique<ChargePlayerBullet01>();

		break;
	}
	bullet->Initialize(modelBullet_.get(), worldPosition, direction, targetEnemy);

	//リストに登録
	playerBullets_.push_back(std::move(bullet));
}

void PlayerBulletManager::RegisterToCollisionManager(CollisionManager* collisionManager)
{
	for (std::unique_ptr<BasePlayerBullet>& bullet : playerBullets_) 
	{
		collisionManager->AddCollider(bullet.get());
	}
}
