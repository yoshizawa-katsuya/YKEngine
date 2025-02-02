#include "Ice.h"
#include "Rigid3dObject.h"
#include "Camera.h"
#include "Vector.h"

void Ice::Initialize(BaseModel* model_) {
    object_ = std::make_unique<Rigid3dObject>();
    object_->Initialize(model_);
    worldTransform_.Initialize();
}

void Ice::Update() {
    worldTransform_.UpdateMatrix();
}

void Ice::Draw() {
    object_->WorldTransformUpdate(worldTransform_);
    object_->Draw();
}
