#include "Star.h"
#include "Rigid3dObject.h"
#include "Camera.h"
#include "Vector.h"

void Star::Initialize(BaseModel* model_, const Vector3& position) {
    object_ = std::make_unique<Rigid3dObject>();
    object_->Initialize(model_);
    worldTransform_.Initialize();
    worldTransform_.translation_ = position;
}

void Star::Update() {
    worldTransform_.UpdateMatrix();

}

void Star::Draw(Camera* camera) {
    object_->CameraUpdate(camera);
    object_->Draw();
}