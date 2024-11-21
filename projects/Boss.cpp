#include "Boss.h"
#include "imgui/imgui.h"

void Boss::Initialize(const std::vector<BaseModel*>& models)
{

	models_ = models;
	worldTransforms_.resize(models.size());
	for (auto& transform : worldTransforms_) {
		transform.Initialize();
		transform.UpdateMatrix();
	}
	worldTransforms_[0].rotation_ = { 0.0f,1.5f,0.0f };
	worldTransforms_[0].translation_ = { 0.0f,0.0f,65.0f };
	worldTransforms_[1].parent_ = &worldTransforms_[0];
	worldTransforms_[2].parent_ = &worldTransforms_[1];
	worldTransforms_[3].parent_ = &worldTransforms_[1];
	worldTransforms_[4].parent_ = &worldTransforms_[0];
	worldTransforms_[5].parent_ = &worldTransforms_[0];

}

void Boss::Update()
{

	for (auto& transform : worldTransforms_) {
		transform.UpdateMatrix();
	}

#ifdef _DEBUG

	ImGui::Begin("Boss");
	for (size_t i = 0; i < models_.size(); ++i) {
		if (ImGui::TreeNode(("Model" + std::to_string(i)).c_str())) {
			ImGui::ColorEdit4("color", &models_[i]->GetMaterialDataAddress().color.x);
			ImGui::DragFloat3("translate", &worldTransforms_[i].translation_.x, 0.01f);
			ImGui::DragFloat3("rotate", &worldTransforms_[i].rotation_.x, 0.01f);
			ImGui::DragFloat3("scale", &worldTransforms_[i].scale_.x, 0.01f);
			ImGui::TreePop();
		}
	}
	ImGui::End();

#endif // _DEBUG	

}

void Boss::Draw(Camera* camera)
{


	for (size_t i = 0; i < models_.size(); ++i) {
		models_[i]->Draw(worldTransforms_[i], camera);
	}

}
