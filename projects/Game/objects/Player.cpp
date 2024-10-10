#include "Player.h"
#include "imgui/imgui.h"

void Player::Initialize(Model* model) {

	model_ = model;
	
	//手前を向かせている
	transform_.rotate = { 0.0f, 3.15f, 0.0f };
	
}

void Player::Update() {

#ifdef _DEBUG

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Model")) {
		ImGui::ColorEdit4("color", &model_->GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);
		ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.01f);
		ImGui::DragFloat3("scale", &transform_.scale.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();

#endif // _DEBUG	

}

void Player::Draw(Camera* camera) {

	model_->Draw(transform_, camera);

	
}

