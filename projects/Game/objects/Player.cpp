#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"

void Player::Initialize(BaseModel* model) {

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();

}

void Player::Update() {

	worldTransform_.UpdateMatrix();

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

	
}

void Player::Draw(Camera* camera) {

	object_->Update(worldTransform_, camera);
	object_->Draw();

}