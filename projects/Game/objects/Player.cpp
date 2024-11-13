#include "Player.h"
#include "imgui/imgui.h"

void Player::Initialize(BaseModel* model) {

	model_ = model;
	
	animation_ = std::make_unique<Animation>();
	
	animation_->LoadAnimationFile("./resources/simpleSkin", "simpleSkin.gltf");

	worldTransform_.Initialize();

	worldTransform_.UpdateMatrix();

}

void Player::Update() {

	worldTransform_.UpdateMatrix();

	animation_->Update();

	model_->Update(animation_.get());

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