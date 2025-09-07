#include "Goal.h"
#include <numbers>

void Goal::Initialize(BaseModel* model)
{
	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float>;
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Goal::Update()
{
	worldTransform_.rotation_.z += 0.05f;
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Goal::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}
