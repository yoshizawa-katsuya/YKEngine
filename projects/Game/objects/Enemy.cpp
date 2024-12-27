#include "Enemy.h"
#include <numbers>
#include "Rigid3dObject.h"

Enemy::Enemy()
{
	radius_ = 0.5f;
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(BaseModel* model, const Vector3& translate)
{
	Collider::Initialize(model);

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.translation_ = translate;
}
/*
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
*/