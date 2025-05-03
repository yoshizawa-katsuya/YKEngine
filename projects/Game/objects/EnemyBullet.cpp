#include "EnemyBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"

void EnemyBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);
	// テクスチャ読み込み
	textureHandle_ = TextureManager::GetInstance()->Load("./Resources/red.png");

	worldTransform_.Initialize();
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

	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void EnemyBullet::OnCollision() { isDead_ = true; }

void EnemyBullet::Draw(Camera* camera) {

	// 3Dモデルを描画
	object_->CameraUpdate(camera);
	object_->Draw(textureHandle_);

}

Vector3 EnemyBullet::GetWorldPosition() {

	return worldTransform_.GetWorldPosition();
}