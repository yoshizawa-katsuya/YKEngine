#include "Player.h"
#include "Input.h"
#include <numbers>
#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void Player::Initialize(BaseModel* model) {

	object_ = std::make_unique<My3dObject>();
	object_->Initialize(model);

	input_ = Input::GetInstance();

	worldTransform_.Initialize();

	pose_ = PlayerPose::Base;
	direction_ = PlayerDirection::Front;

	kAngle_=std::numbers::pi_v<float>/4.0f;
}

void Player::Update() {


#ifdef USE_IMGUI

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Model")) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);
		int poseInt = static_cast<int>(pose_);
		ImGui::DragInt("pose", &poseInt, 1.0f, 0, 5);
		pose_ = static_cast<PlayerPose>(poseInt);

		ImGui::TreePop();
	}
	ImGui::End();


#endif // USE_IMGUI	

	ChangePose();
	ChangeDirection();

	UpdateColorForDebug();

	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);

}

void Player::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();

}

void Player::ChangePose()
{
	// ポーズ切り替え
    // デフォルト
	pose_ = PlayerPose::Base;

	// 押されたらポーズ変更
	if (input_->PushKey(DIK_DOWNARROW))
	{
		pose_ = PlayerPose::Squat;
	}
	else if (input_->PushKey(DIK_1))
	{
		pose_ = PlayerPose::A;
	}
	else if (input_->PushKey(DIK_2))
	{
		pose_ = PlayerPose::B;
	}
	else if (input_->PushKey(DIK_3))
	{
		pose_ = PlayerPose::C;
	}
	else if (input_->PushKey(DIK_4))
	{
		pose_ = PlayerPose::D;
	}
}

void Player::ChangeDirection()
{
	// 向き切り替え
	int dir = static_cast<int>(direction_);
	if (input_->TriggerKey(DIK_LEFTARROW))
		dir--;
	if (input_->TriggerKey(DIK_RIGHTARROW))
		dir++;

	// 範囲制限（0～2）
	dir = std::clamp(dir, 0, 2);
	direction_ = static_cast<PlayerDirection>(dir);

	// -1, 0, 1 に変換
	int offset = dir - 1;

	// 目標角度
	float targetRot = offset * kAngle_;

	// 現在角度
	float currentRot = worldTransform_.rotation_.y;

	// スムーズ回転
	float speed = 20.0f;
	float deltaTime = 1.0f / 60.0f; // 仮

	worldTransform_.rotation_.y += (targetRot - currentRot) * speed * deltaTime;
	if (fabs(targetRot - worldTransform_.rotation_.y) < 0.001f)
	{
		worldTransform_.rotation_.y = targetRot;
	}
}

void Player::UpdateColorForDebug()
{
	static const Vector4 kPoseColors[] =
	{
		{1,1,1,1}, // Base
		{0,0,0,1}, // Squat
		{1,0,0,1}, // A
		{0,1,0,1}, // B
		{0,0,1,1}, // C
		{1,1,0,1}, // D
	};

	object_->SetColor(kPoseColors[static_cast<int>(pose_)]);
}
