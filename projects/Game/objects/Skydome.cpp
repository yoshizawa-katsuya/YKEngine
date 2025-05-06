#include "Skydome.h"
#include "cassert"

void Skydome::Initialize(BaseModel* model) {

	// NULLポインタチェック
	assert(model);

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);
	object_->SetEnableLighting(false);

	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Skydome::Update() {

}

void Skydome::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();

}