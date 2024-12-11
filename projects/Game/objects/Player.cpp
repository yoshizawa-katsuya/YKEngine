#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"
#include "Lerp.h"
#include "Easing.h"

Player::Player()
	: input_(Input::GetInstance())
	, speed_(0.1f)
	, lrDirection_(LRDirection::kRight)
	, angleCompletionRate_(0.2f)
	, turnTimer_(1.0f)
{
}

void Player::Initialize(BaseModel* model) {

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= speed_;
		if (lrDirection_ != LRDirection::kLeft) {
			lrDirection_ = LRDirection::kLeft;
			targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += speed_;
		if (lrDirection_ != LRDirection::kRight) {
			lrDirection_ = LRDirection::kRight;
			targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}

	// 旋回制御
	if (turnTimer_ < 1.0f) {
		turnTimer_ += 1.0f / 12.0f;

		if (turnTimer_ > 1.0f) {
			turnTimer_ = 1.0f;
		}
		// 自キャラの角度を設定する
		//worldTransform_.rotation_.y = turnFirstRotationY_ * EaseOut(turnTimer_) + destinationRotationY * (1 - EaseOut(turnTimer_));
		worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, targetAngle_, EaseOut(turnTimer_));

	}

#ifdef _DEBUG

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Model")) {
		ImGui::ColorEdit4("color", &object_->GetModel().GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();


#endif // _DEBUG	

	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);

}

void Player::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();

}