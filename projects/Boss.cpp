#include "Boss.h"
#include "imgui/imgui.h"

void Boss::Initialize(BaseModel* model)
{

	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.translation_ = { 0.0f,0.0f,65.0f };
	worldTransform_.rotation_ = { 0.0f,1.5f,0.0f };

	worldTransform_.UpdateMatrix();

}

void Boss::Update()
{

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("Boss");
	if (ImGui::TreeNode("Model1")) {
		ImGui::ColorEdit4("color", &model_->GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();

#endif // _DEBUG	

}

void Boss::Draw(Camera* camera)
{


	model_->Draw(worldTransform_, camera);

}
