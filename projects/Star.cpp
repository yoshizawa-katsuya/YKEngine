#include "Star.h"
#include "Rigid3dObject.h"
#include "Camera.h"
#include "Vector.h"

void Star::Initialize(BaseModel* model_) {
    object_ = std::make_unique<Rigid3dObject>();
    object_->Initialize(model_);
    worldTransform_.Initialize();
    worldTransform_.translation_ = { 1.0f, 1.0f, 0.0f };
    worldTransform_.scale_ = { 0.1f, 0.1f, 0.1f };
    worldTransform_.rotation_ = { 0.0f, 3.0f, 0.0f };
    input_ = Input::GetInstance();
}

void Star::Update() {
    worldTransform_.UpdateMatrix();

}

void Star::Draw(Camera*camera) {
    object_->CameraUpdate(camera);
    object_->WorldTransformUpdate(worldTransform_);
    object_->Draw();
}
