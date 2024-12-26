#include "Enemy.h"
#include <numbers>
#include "Rigid3dObject.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(BaseModel* model, const Vector3& translate)
{
	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.translation_ = translate;
}

void Enemy::Update()
{
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Enemy::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}
