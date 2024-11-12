#include "Object3d.h"
#include "imgui/imgui.h"

void Object3d::Initialize(BaseModel* model)
{

	model_ = model;
	//camera_ = camera;

}

void Object3d::Update(const std::string& label)
{
	ImGui::Begin("Object3d");
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::ColorEdit4("color", &model_->GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();
}

void Object3d::Draw(Camera* camera)
{
	if (model_) {
		model_->Draw(worldTransform_, camera);
	}
}
