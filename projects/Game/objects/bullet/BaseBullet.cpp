#include "BaseBullet.h"

void BaseBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle)
{
	BaseCharacter::Initialize(model);

	//テクスチャ読み込み
	textureHandle_ = textureHandle;

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	SetRadius(0.5f);
}

void BaseBullet::Update()
{
	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//移動処理
	Move();

	BaseCharacter::Update();
}

void BaseBullet::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw(textureHandle_);
}

void BaseBullet::Move()
{
	//座標を移動させる
	worldTransform_.translation_ += velocity_;
}
