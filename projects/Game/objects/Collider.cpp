#include "Collider.h"
#include "Matrix.h"
#include "Camera.h"

void Collider::Initialize()
{
	worldTransform_.Initialize();
}

void Collider::Update()
{
	worldTransform_.UpdateMatrix();
}

bool Collider::IsVisible(Camera* camera)
{
	Vector3 clipPosition = Transform(worldTransform_.GetWorldPosition(), camera->GetViewProjection());

	//NDCの範囲内にあるかチェック
	if (clipPosition.x < -1.1f || clipPosition.x > 1.1f ||
		clipPosition.y < -1.1f || clipPosition.y > 1.1f ||
		clipPosition.z < 0.0f || clipPosition.z > 1.0f) {
		return false; // 範囲外
	}

	return true; // 範囲内
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
