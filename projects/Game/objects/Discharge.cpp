#include "Discharge.h"
#include <numbers>


void Discharge::Initialize(BaseModel* model) {
    object_ = std::make_unique<Rigid3dObject>();
    object_->Initialize(model);

    
	uvTransform_.scale = { 3.0f, 1.0f, 0.1f };
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 4.0f, 4.0f, 0.10f };
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
	object_->SetColor({ 1.0f, 0.95f, 0.45f, 0.65f });
	object_->SetEnableLighting(false);
	object_->SetUVTransform(uvTransform_);
}

void Discharge::Update() {
	

	object_->SetUVTransform(uvTransform_);
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Discharge::Draw(Camera* camera) {
	object_->CameraUpdate(camera);
	object_->Draw();
}

void Discharge::SetPosition(const Vector3& position) {
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}