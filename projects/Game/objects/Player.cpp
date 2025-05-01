#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"

void Player::Initialize(BaseModel* model) {

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);
	object_->SetEnableLighting(false);

	worldTransform_.Initialize();

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
	x_ += 0.0002f;
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
	object_->SetUVTransform({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { x_, 0.0f, 0.0f });

}

void Player::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();

}