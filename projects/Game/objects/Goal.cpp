#include "Goal.h"
#include <numbers>

void Goal::Initialize(BaseModel* model)
{
	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);


	uvTransform_.scale = { 1.0f, 1.0f, 1.0f };
	uvTransform_.rotation.x = std::numbers::pi_v<float>;
	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
	object_->SetColor({ 0.2f, 0.2f, 1.0f, 1.0f });
	object_->SetEnableLighting(false);
	object_->SetUVTransform(uvTransform_);
}

void Goal::Update()
{
	uvTransform_.translation.x += 0.01f;
	if (uvTransform_.translation.x > 1.0f)
	{
		uvTransform_.translation.x -= 1.0f;
	}
	object_->SetUVTransform(uvTransform_);
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Goal::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}

void Goal::SetPosition(const Vector3& position)
{
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y -= 1.2f;
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}
