#include "Laser.h"
#include "ModelPlatform.h"
#include "TransformHelpers.h"

using namespace YKEngine;

void Laser::Initialize(WorldTransform* leftPlayerWorldTransform, WorldTransform* rightPlayerWorldTransform)
{
	//モデルの生成
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	std::shared_ptr<BaseModel> model = modelPlatform->CreateRigidModel("./resources/laser", "laser.obj");

	// 3Dオブジェクトの初期化
	object_ = std::make_unique<My3dObject>();
	object_->Initialize(model.get());

	// Transformの初期化
	worldTransform_.Initialize();

	// プレイヤーのTransformを設定
	leftPlayerWorldTransform_ = leftPlayerWorldTransform;
	rightPlayerWorldTransform_ = rightPlayerWorldTransform;

}

void Laser::Update()
{
	// 左右の自機の中間点に移動
	Move();

	// 自機を向く
	Rotate();

	// 左右の自機間の距離を計算してZ軸方向のスケールを設定
	UpdateLength();

	// ワールド変換行列の更新
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Laser::Draw(YKEngine::Camera * camera)
{
	//レーザーの描画
	object_->CameraUpdate(camera);
	object_->Draw();
}

void Laser::Move()
{
	// 左右の自機の中間点に移動
	Vector3 targetPosition = (leftPlayerWorldTransform_->translation_ + rightPlayerWorldTransform_->translation_) / 2.0f;	// 左右の自機の中間点
	worldTransform_.translation_ = targetPosition;
}

void Laser::Rotate()
{
	// 自機を向く
	Vector3 direction = leftPlayerWorldTransform_->translation_ - worldTransform_.translation_;	// 自機への方向ベクトル
	Vector3 targetRotate = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction);	// 方向ベクトルから回転角を取得

	worldTransform_.rotation_ = TransformHelpers::NormalizeAngle(targetRotate);
}

void Laser::UpdateLength()
{
	// 左右の自機間の距離を計算してZ軸方向のスケールを設定
	Vector3 direction = rightPlayerWorldTransform_->translation_ - leftPlayerWorldTransform_->translation_;	// 左右の自機への方向ベクトル
	float length = Length(direction);	// 左右の自機間の距離

	worldTransform_.scale_.z = length;	// Z軸方向のスケールを距離に設定
}
