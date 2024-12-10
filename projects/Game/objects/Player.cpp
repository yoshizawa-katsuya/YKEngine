#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"
#include "Skin3dObject.h"

void Player::Initialize(BaseModel* model, Animation* animation) {

	object_ = std::make_unique<Skin3dObject>();
	object_->Initialize(model);

	animation_ = animation;

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

	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);

	animation_->Update();
	object_->AnimationUpdate(animation_);
}

void Player::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();

}