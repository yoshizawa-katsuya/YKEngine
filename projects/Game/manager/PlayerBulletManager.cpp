#include "PlayerBulletManager.h"
#include "ModelPlatform.h"
#include "CollisionManager.h"

void PlayerBulletManager::Initialize()
{
	//テクスチャ読み込み
	textureHandlePlayerBullet_ = TextureManager::GetInstance()->Load("./Resources/black.png");

	//モデル生成
	modelBullet_ = ModelPlatform::GetInstance()->CreateSphere(textureHandlePlayerBullet_, "Bullet");

}

void PlayerBulletManager::Update()
{
	// デスフラグの立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
		});
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Update();
	}
}

void PlayerBulletManager::Draw(Camera* camera)
{
	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Draw(camera);
	}
}

void PlayerBulletManager::AddPlayerBullet(const Vector3& worldPosition, const Vector3& velocity)
{
	//リストに登録する
	//弾を生成し、初期化
	std::unique_ptr<PlayerBullet>& bullet = playerBullets_.emplace_back();
	bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(modelBullet_.get(), worldPosition, velocity, textureHandlePlayerBullet_);
}

void PlayerBulletManager::AddColliders(CollisionManager* collisionManager)
{
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		collisionManager->AddCollider(bullet.get());
	}
}
