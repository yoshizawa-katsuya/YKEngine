#include "SkyDome.h"
#include "imgui/imgui.h"

void SkyDome::Initialize(BaseModel* model)
{

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.5f,1.5f,1.5f };
}

void SkyDome::Update()
{

	worldTransform_.UpdateMatrix();
#ifdef _DEBUG

	ImGui::Begin("skyDome");

	ImGui::DragFloat3("translation_", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation_", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("scale_", &worldTransform_.scale_.x, 0.01f);



	ImGui::End();

#endif // _DEBUG
}

void SkyDome::Draw(Camera* camera)
{


	model_->Draw(worldTransform_, camera);

}
