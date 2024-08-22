#include "Object3d.h"
#include "imgui/imgui.h"

void Object3d::Initialize(Model* model, Camera* camera)
{

	model_ = model;
	camera_ = camera;

}

void Object3d::Update(const std::string& label)
{
	ImGui::Begin("Object3d");
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::ColorEdit4("color", &model_->GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);
		ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.01f);
		ImGui::DragFloat3("scale", &transform_.scale.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();
}

void Object3d::Draw()
{
	if (model_) {
		model_->Draw(transform_, camera_);
	}
}
