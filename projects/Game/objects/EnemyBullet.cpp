#include "EnemyBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"
#include "CollisionTypeIdDef.h"

void EnemyBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) {

	BaseCharacter::Initialize(model);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet));

	// テクスチャ読み込み
	textureHandle_ = textureHandle;

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	velocity_ = velocity;

}

void EnemyBullet::Update() {

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 座標を移動させる
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	BaseCharacter::Update();
}

void EnemyBullet::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();

	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer) || typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet))
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw(textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() {

	return worldTransform_.GetWorldPosition();
}