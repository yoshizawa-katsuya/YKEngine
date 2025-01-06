#include "Bullet.h"

void Bullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity)
{
	Collider::Initialize(model);

	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { radius_, radius_, radius_ };
	velocity_ = velocity;
}

void Bullet::Update()
{
	worldTransform_.translation_ += velocity_;

	Collider::Update();
}

void Bullet::OnCollision()
{
	isAlive_ = false;
}
