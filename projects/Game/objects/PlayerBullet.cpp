#include "PlayerBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"

void PlayerBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);
	//テクスチャ読み込み
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
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

	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);

}

void PlayerBullet::OnCollision() { isDead_ = true; }

void PlayerBullet::Draw(Camera* camera) {

	// 3Dモデルを描画
	object_->CameraUpdate(camera);
	object_->Draw(textureHandle_);

}

Vector3 PlayerBullet::GetWorldPosition() {

	return worldTransform_.GetWorldPosition();
}