#include "PlayerBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"
#include "CollisionTypeIdDef.h"

void PlayerBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) {

	BaseCharacter::Initialize(model);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet));

	//テクスチャ読み込み
	textureHandle_ = textureHandle;

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

}

void PlayerBullet::Update() {

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//座標を移動させる
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	BaseCharacter::Update();

}

void PlayerBullet::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();

	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy) || typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet))
	{
		isDead_ = true;
	}
}

void PlayerBullet::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw(textureHandle_);
}

Vector3 PlayerBullet::GetWorldPosition() {

	return worldTransform_.GetWorldPosition();
}