#include "RailCamera.h"
#include "Matrix.h"
#include "ImGuiManager.h"
#include "imgui/imgui.h"
#include "Curve.h"
#include "TransformHelpers.h"
#include "ModelPlatform.h"
#include "Slerp.h"

void RailCamera::Initialize(Camera* camera, WorldTransform* parent, WorldTransform* playerWorldTransform)
{

	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_.z = -13.0f;
	worldTransform_.parent_ = parent; // 親ワールドトランスフォームを設定

	playerParentWorldTransform_.Initialize();
	playerParentWorldTransform_.translation_.z = 8.0f;
	playerParentWorldTransform_.parent_ = playerWorldTransform;

	camera_ = camera;

	worldTransform_.UpdateMatrix();

	//カメラの更新
	camera_->SetTranslate(worldTransform_.GetWorldPosition());
	camera_->SetRotate(worldTransform_.parent_->rotation_);
	camera_->Update();
}

void RailCamera::Update() {

	worldTransform_.UpdateMatrix();

	switch (phase_)
	{
	case RailCamera::Phase::Main:
		UpdateMain();
		break;
	case RailCamera::Phase::GameOver:
		UpdateGameOver();
		break;
	default:
		break;
	}

#ifdef _DEBUG

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -20.0f, 20.0f);
	ImGui::End();

#endif // _DEBUG	
}

void RailCamera::SetGameOver()
{
	phase_ = Phase::GameOver;
	t_ = 0.0f;
}

void RailCamera::CreateTargetRotationFromPlayer(const Vector3& direction)
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
	playerParentWorldTransform_.UpdateMatrix();

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
	camera_->SetTranslate(SlerpTranslateByCenterAxis(playerParentWorldTransform_.parent_->GetWorldPosition(), Vector3{ 0.0f, 1.0f, 0.0f }, worldTransform_.GetWorldPosition(), playerParentWorldTransform_.GetWorldPosition(), t_));
	// プレイヤーの正面を向くようにカメラの回転を調整する
	//Vector3 targetAngle = playerParentWorldTransform_.parent_->rotation_ + Vector3(-playerParentWorldTransform_.parent_->rotation_.x * 2.0f, std::numbers::pi_v<float>, 0.0f);
	//targetAngle += {-worldTransform_.parent_->rotation_.x, worldTransform_.parent_->rotation_.y, worldTransform_.parent_->rotation_.z}; // 親の回転を加算
	camera_->SetRotate(LerpAngle(worldTransform_.parent_->rotation_, targetRotation_, t_));
	camera_->Update();

}
