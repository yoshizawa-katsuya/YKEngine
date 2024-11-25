#include "Ground.h"
#include "imgui/imgui.h"

void Ground::Initialize(BaseModel* model)
{
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 10.0f,1.0f,10.0f };
}

void Ground::Update()
{

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("Ground");
	
	ImGui::DragFloat3("translation_", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation_", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("scale_", &worldTransform_.scale_.x, 0.01f);
			
		
	
	ImGui::End();

#endif // _DEBUG	

}

void Ground::Draw(Camera* camera)
{

	model_->Draw(worldTransform_, camera);

}
