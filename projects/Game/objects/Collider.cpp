#include "Collider.h"

void Collider::Initialize()
{
	worldTransform_.Initialize();
}

void Collider::Update()
{
	worldTransform_.UpdateMatrix();
}

Vector3 Collider::GetCenterPosition()
{
	return worldTransform_.GetWorldPosition();
}

void Collider::SetRadius(float radius)
{
	radius_ = radius;

	worldTransform_.scale_ = { radius_, radius_, radius_ };
}
