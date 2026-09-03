#include "BaseCollider.h"
#include "Camera.h"

using namespace YKEngine;

void BaseCollider::Initialize()
{
	worldTransform_.Initialize();
}

void BaseCollider::Update()
{
	worldTransform_.UpdateMatrix();
}

bool BaseCollider::IsVisible(Camera* camera)
{
	Vector4 clipPosition = ClipPosition(camera);

	// カメラ後ろ判定は w で
	if (clipPosition.w < 0.0f) 
	{
		return false; // カメラの後ろにある
	}

	// NDC変換
	assert(clipPosition.w != 0.0f);
	clipPosition.x /= clipPosition.w;
	clipPosition.y /= clipPosition.w;
	clipPosition.z /= clipPosition.w;

	//NDCの範囲内にあるかチェック
	if (clipPosition.x < -1.1f || clipPosition.x > 1.1f ||
		clipPosition.y < -1.1f || clipPosition.y > 1.1f ||
		clipPosition.z < 0.0f || clipPosition.z > 1.0f) {
		return false; // 範囲外
	}

	return true; // 範囲内
}

Vector4 BaseCollider::ClipPosition(Camera* camera)
{
	Vector3 vector = worldTransform_.GetWorldPosition();
	Matrix4x4 matrix = camera->GetViewProjection();
	Vector4 clipPosition;

	clipPosition.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	clipPosition.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	clipPosition.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	clipPosition.w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	return clipPosition;
}

Vector3 BaseCollider::GetCenterPosition()
{
	return worldTransform_.GetWorldPosition();
}
