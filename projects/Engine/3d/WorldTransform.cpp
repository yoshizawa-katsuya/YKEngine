#include "WorldTransform.h"
#include "Matrix.h"

void WorldTransform::Initialize()
{
	worldMatrix_ = MakeIdentity4x4();
}

void WorldTransform::UpdateMatrix()
{
	//スケール、回転、平行移動を合成して行列を計算する
	worldMatrix_ = MakeAffineMatrix(scale_, rotation_, translation_);

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		worldMatrix_ = Multiply(worldMatrix_, parent_->worldMatrix_);
	}
}

Vector3 WorldTransform::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worlsPos;
	// ワールド行列の平行移動成分を取得
	worlsPos.x = worldMatrix_.m[3][0];
	worlsPos.y = worldMatrix_.m[3][1];
	worlsPos.z = worldMatrix_.m[3][2];

	return worlsPos;
}
