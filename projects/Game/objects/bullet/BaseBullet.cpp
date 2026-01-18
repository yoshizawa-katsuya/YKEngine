#include "BaseBullet.h"
#include "TransformHelpers.h"

using namespace YKEngine;

void BaseBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity)
{
	BaseCharacter::Initialize(model);

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//移動方向に向ける
	Rotate();

	radius_ = 0.5f; //仮の当たり判定半径
}

void BaseBullet::Update()
{
	//時間経過でデス
	if (--deathTimer_ <= 0) 
	{
		isDead_ = true;
	}

	//移動処理
	Move();

	BaseCharacter::Update();
}

void BaseBullet::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}

void BaseBullet::Move()
{
	//座標を移動させる
	worldTransform_.translation_ += velocity_;
}

void BaseBullet::Rotate()
{
	//移動方向に向ける
	worldTransform_.rotation_ = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, velocity_);
}
