#include "Skydome.h"
#include "cassert"

void Skydome::Initialize(RigidModel* model) {

	assert(model);

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
}

void Skydome::Update() { 
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Skydome::Draw(Camera* camera) {
	object_->CameraUpdate(camera);
	object_->Draw();
}