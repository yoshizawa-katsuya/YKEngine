#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"

Player::Player()
	: input_(Input::GetInstance())
	, speed_(0.1f)
	, lrDirection_(LRDirection::kRight)
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
			lrDirection_ = LRDirection::kRight;
		}
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += speed_;
		if (lrDirection_ != LRDirection::kRight) {
			lrDirection_ = LRDirection::kLeft;
		}
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