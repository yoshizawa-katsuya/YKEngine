#include "SphereCollider.h"

void SphereCollider::Initialize()
{
	BaseCollider::Initialize();

	shapeType_ = ColliderShapeType::kSphere;
}

void SphereCollider::SetRadius(float radius)
{
	radius_ = radius;

	worldTransform_.scale_ = { radius_, radius_, radius_ };
}
