#include "BasePlayer.h"
#include "TransformHelpers.h"
#include "CollisionManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void BasePlayer::Initialize(BaseModel* model)
{
	// 球コライダーの初期化
	SphereCollider::Initialize();

	// 球コライダーの半径を設定
	SetRadius(0.5f);

	//オブジェクトの生成
	object_ = std::make_unique<My3dObject>();
	object_->Initialize(model);

	input_ = Input::GetInstance();

	//Colliderの種別IDをプレイヤーに設定
	BaseCollider::SetTypeID(CollisionTypeIdDef::kPlayer);

	//衝突マネージャーに登録
	CollisionManager::GetInstance()->AddSphereCollider(this);
}

void BasePlayer::Update()
{


#ifdef USE_IMGUI

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Model")) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();


#endif // USE_IMGUI	

	// 移動
	Move();

	// 回転
	Rotate();

	// ワールド変換行列の更新
	SphereCollider::Update();
	object_->WorldTransformUpdate(worldTransform_);

}

void BasePlayer::Draw(Camera* camera)
{

	object_->CameraUpdate(camera);
	object_->Draw();

}

void BasePlayer::Rotate()
{
	// 自分ではない方の自機を向く
	Vector3 direction = otherPlayerWorldTransform_->GetWorldPosition() - worldTransform_.GetWorldPosition();	// 自分ではない方の自機への方向ベクトル
	Vector3 targetRotate = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction);	// 方向ベクトルから回転角を取得

	worldTransform_.rotation_ = TransformHelpers::NormalizeAngle(targetRotate);
}
