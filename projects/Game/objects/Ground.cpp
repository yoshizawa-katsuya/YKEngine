#include "Ground.h"
#include "cassert"

void Ground::Initialize(RigidModel* model) {
	
	assert(model);

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
}

void Ground::Update() { 
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Ground::Draw(Camera* camera) {
	object_->CameraUpdate(camera);
	object_->Draw();
}