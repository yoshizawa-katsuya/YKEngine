#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"

void Player::Initialize(BaseModel* model) {

	BaseCharacter::Initialize(model);

}

void Player::Update() {

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

	BaseCharacter::Update();

}

void Player::Draw(Camera* camera) {

	BaseCharacter::Draw(camera);

}