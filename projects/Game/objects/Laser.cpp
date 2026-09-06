#include "Laser.h"
#include "ModelPlatform.h"
#include "TransformHelpers.h"
#include "CollisionManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI


using namespace YKEngine;

void Laser::Initialize(WorldTransform* leftPlayerWorldTransform, WorldTransform* rightPlayerWorldTransform)
{
	// OBBコライダーの初期化
	OBBCollider::Initialize();

	worldTransform_.scale_.x = 0.1f;	// X軸方向のスケールを設定
	worldTransform_.scale_.y = 0.1f;	// Y軸方向のスケールを設定

	//モデルの生成
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	std::shared_ptr<BaseModel> model = modelPlatform->CreateRigidModel("./resources/laser", "laser.obj");

	// 3Dオブジェクトの初期化
	object_ = std::make_unique<My3dObject>();
	object_->Initialize(model.get());

	// プレイヤーのTransformを設定
	leftPlayerWorldTransform_ = leftPlayerWorldTransform;
	rightPlayerWorldTransform_ = rightPlayerWorldTransform;

	//Colliderの種別IDをプレイヤーに設定
	BaseCollider::SetTypeID(CollisionTypeIdDef::kLaser);

	//衝突マネージャーに登録
	CollisionManager::GetInstance()->AddOBBCollider(this);
}

void Laser::Update()
{
	// 左右の自機の中間点に移動
	Move();

	// 自機を向く
	Rotate();

	// 左右の自機間の距離を計算してZ軸方向のスケールを設定
	UpdateLength();

	UpdateEnergy();

	// ワールド変換行列の更新
	OBBCollider::Update();
	object_->WorldTransformUpdate(worldTransform_);

#ifdef USE_IMGUI
	ImGui::Begin("Laser");
	// ImGuiでエネルギー量を表示
	ImGui::Text("Laser Energy: %.2f", energy_);
	ImGui::End();
#endif // USE_IMGUI

}

void Laser::Draw(YKEngine::Camera * camera)
{
	// エネルギー量が0以下の場合は描画しない
	if (energy_ <= 0.0f)
	{
		return;
	}
	//レーザーの描画
	object_->CameraUpdate(camera);
	object_->Draw();
}

void Laser::Move()
{
	// 左右の自機の中間点に移動
	Vector3 targetPosition = (leftPlayerWorldTransform_->GetWorldPosition() + rightPlayerWorldTransform_->GetWorldPosition()) / 2.0f;	// 左右の自機の中間点
	worldTransform_.translation_ = targetPosition;
}

void Laser::Rotate()
{
	// 自機を向く
	Vector3 direction = rightPlayerWorldTransform_->GetWorldPosition() - worldTransform_.GetWorldPosition();	// 自機への方向ベクトル
	Vector3 targetRotate = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction);	// 方向ベクトルから回転角を取得

	worldTransform_.rotation_ = TransformHelpers::NormalizeAngle(targetRotate);
}

void Laser::UpdateLength()
{
	// 左右の自機間の距離を計算してZ軸方向のスケールを設定
	Vector3 direction = rightPlayerWorldTransform_->GetWorldPosition() - leftPlayerWorldTransform_->GetWorldPosition();	// 左右の自機への方向ベクトル
	float length = Length(direction);	// 左右の自機間の距離

	worldTransform_.scale_.z = length / 2;	// Z軸方向のスケールを距離に設定
}

void Laser::UpdateEnergy()
{
	energy_ -= kEnergyConsumptionRate * worldTransform_.scale_.z * 0.01f; // エネルギー量を消費する

	energy_ += 0.2f; // エネルギー量を増加させる
	energy_ = std::clamp(energy_, 0.0f, kMaxEnergy); // エネルギー量を最大値で制限

	if (energy_ <= 0.0f)
	{
		// エネルギーが0以下になった場合の処理
		BaseCollider::SetTypeID(CollisionTypeIdDef::kDefault); // 衝突判定を無効化
	}
	else
	{
		BaseCollider::SetTypeID(CollisionTypeIdDef::kLaser); // 衝突判定を有効化
	}
}
