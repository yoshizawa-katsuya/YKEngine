#include "Player.h"
#include "imgui/imgui.h"

void Player::Initialize(Model* model) {

	model_ = model;
	
	worldTransform_.Initialize();

	worldTransform_.UpdateMatrix();

}

void Player::Update() {

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Model")) {
		ImGui::ColorEdit4("color", &model_->GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();

#endif // _DEBUG	

	
}

void Player::Draw(Camera* camera) {

	
	model_->Draw(worldTransform_, camera);
	
}