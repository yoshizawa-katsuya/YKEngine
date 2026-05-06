#include "PlayerBulletManager.h"
#include "ModelPlatform.h"
#include "CollisionManager.h"
#include "bullet/PlayerBullet01.h"
#include "bullet/ChargePlayerBullet01.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void PlayerBulletManager::Initialize()
{

	//モデル生成
	modelBullet_ = ModelPlatform::GetInstance()->CreateRigidModel("./Resources/playerBullet", "PlayerBullet.obj");

	//調整項目をjsonに登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->CreateGroup(JsonKey::Bullet::Player::kGroupName);
	globalVariables->AddItem(JsonKey::Bullet::Player::kGroupName, JsonKey::Bullet::kRotateSpeed, 0.3f);

	globalVariables->CreateGroup(JsonKey::Bullet::Player::Charge01::kGroupName);
	globalVariables->AddItem(JsonKey::Bullet::Player::Charge01::kGroupName, JsonKey::Bullet::kRadius, 1.0f);
	globalVariables->AddItem(JsonKey::Bullet::Player::Charge01::kGroupName, JsonKey::Bullet::kAttackPower, 3);
}

void PlayerBulletManager::Update()
{
	// デスフラグの立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<BasePlayerBullet>& bullet) {
		if (bullet->IsDead()) {
			CollisionManager::GetInstance()->RemoveSphereCollider(bullet.get());
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
	//弾を生成し、初期化
	//bulletTypeに対応する弾の生成関数を取得
	const std::unordered_map<PlayerBulletType, PlayerBulletFactory>& playerBulletFactoryMap = GetPlayerBulletFactoryMap();
	auto it = playerBulletFactoryMap.find(bulletType);
	if (it == playerBulletFactoryMap.end())
	{
		assert(false); // 弾の種類が見つからない場合はエラー
		return;
	}

	std::unique_ptr<BasePlayerBullet> bullet = it->second();
	
	bullet->Initialize(modelBullet_.get(), worldPosition, direction, targetEnemy);

	//リストに登録
	playerBullets_.push_back(std::move(bullet));

	CollisionManager::GetInstance()->AddSphereCollider(playerBullets_.back().get());
}

const std::unordered_map<PlayerBulletType, PlayerBulletManager::PlayerBulletFactory>& PlayerBulletManager::GetPlayerBulletFactoryMap() const
{
	static const std::unordered_map<PlayerBulletType, PlayerBulletFactory> playerBulletFactoryMap =
	{
		{PlayerBulletType::kNormal, []() { return std::make_unique<PlayerBullet01>(); }},
		{PlayerBulletType::kCharge, []() { return std::make_unique<ChargePlayerBullet01>(); }},
	};
	return playerBulletFactoryMap;
}
