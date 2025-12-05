#include "RailCamera.h"
#include "Matrix.h"
#include "Curve.h"
#include "TransformHelpers.h"
#include "ModelPlatform.h"
#include "Slerp.h"
#include "Camera.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void RailCamera::Initialize(WorldTransform* parent, WorldTransform* playerWorldTransform)
{

	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_.z = -13.0f;
	worldTransform_.parent_ = parent; // 親ワールドトランスフォームを設定

	targetParentWorldTransform_.Initialize();
	targetParentWorldTransform_.translation_.z = 8.0f;
	targetParentWorldTransform_.parent_ = playerWorldTransform;

	worldTransform_.UpdateMatrix();

	//カメラの初期設定
	camera_ = std::make_unique<Camera>();
	camera_->SetTranslate(worldTransform_.GetWorldPosition());
	camera_->SetRotate(worldTransform_.parent_->rotation_);
	camera_->Update();
}

void RailCamera::Update() {

	worldTransform_.UpdateMatrix();

	switch (phase_)
	{
	case RailCamera::Phase::kMain:
		UpdateMain();
		break;
	case RailCamera::Phase::kGameOver:
		UpdateGameOver();
		break;
	case RailCamera::Phase::kClearScene:
		UpdateClearScene();
		break;
	default:
		break;
	}

#ifdef USE_IMGUI

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -20.0f, 20.0f);
	ImGui::End();

#endif // USE_IMGUI	
}

void RailCamera::SetGameOver()
{
	phase_ = Phase::kGameOver;
	t_ = 0.0f;
}

void RailCamera::SetClearScene()
{
	phase_ = Phase::kClearScene;

	targetParentWorldTransform_.UpdateMatrix();

	//カメラの更新
	camera_->SetTranslate(SlerpTranslateByCenterAxis(targetParentWorldTransform_.parent_->GetWorldPosition(), Vector3{ 0.0f, 1.0f, 0.0f }, worldTransform_.GetWorldPosition(), targetParentWorldTransform_.GetWorldPosition(), -1.0f, 0.9f));
	// プレイヤーの正面を向くようにカメラの回転を調整する
	camera_->SetRotate(LerpAngle(worldTransform_.parent_->rotation_, targetRotation_, 0.9f));
	camera_->Update();
}

void RailCamera::CreateTargetRotationFromDirection(const Vector3& direction)
{
	targetRotation_ = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction);
}

void RailCamera::UpdateMain()
{
	//カメラの更新
	camera_->SetTranslate(worldTransform_.GetWorldPosition());
	camera_->SetRotate(worldTransform_.parent_->rotation_);
	camera_->Update();
}

void RailCamera::UpdateGameOver()
{
	targetParentWorldTransform_.UpdateMatrix();

	// t_が1.0f以上になったら処理を終了する
	if (t_ >= 1.0f)
	{
		return;
	}
	// t_を0.0fから1.0fまで徐々に増加させる
	t_ += 0.03f;
	if (t_ > 1.0f)
	{
		t_ = 1.0f;
	}

	//カメラの更新
	// カメラの位置をプレイヤーの位置に徐々に近づける
	camera_->SetTranslate(SlerpTranslateByCenterAxis(targetParentWorldTransform_.parent_->GetWorldPosition(), Vector3{ 0.0f, 1.0f, 0.0f }, worldTransform_.GetWorldPosition(), targetParentWorldTransform_.GetWorldPosition(), t_));
	// プレイヤーの正面を向くようにカメラの回転を調整する
	camera_->SetRotate(LerpAngle(worldTransform_.parent_->rotation_, targetRotation_, t_));
	camera_->Update();

}

void RailCamera::UpdateClearScene()
{
	targetParentWorldTransform_.UpdateMatrix();

	//カメラの更新
	Vector3 targetTranslate = SlerpTranslateByCenterAxis(targetParentWorldTransform_.parent_->GetWorldPosition(), Vector3{ 0.0f, 1.0f, 0.0f }, worldTransform_.GetWorldPosition(), targetParentWorldTransform_.GetWorldPosition(), -1.0f, 0.9f);
	camera_->SetTranslate(Lerp(camera_->GetTranslate(), targetTranslate, 0.1f));
	// プレイヤーの正面を向くようにカメラの回転を調整する
	targetRotation_ = LerpAngle(worldTransform_.parent_->rotation_, targetRotation_, 0.9f);
	camera_->SetRotate(LerpAngle(camera_->GetRotate(), targetRotation_, 0.03f));
	camera_->Update();
}
