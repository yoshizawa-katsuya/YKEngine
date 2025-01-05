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

void WorldTransform::UpdateMatrix(Matrix4x4 matrix)
{
	worldMatrix_ = matrix;
	//スケール、回転、平行移動を合成して行列を計算する
	worldMatrix_ = Multiply(worldMatrix_, MakeAffineMatrix(scale_, rotation_, translation_));

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		worldMatrix_ = Multiply(worldMatrix_, parent_->worldMatrix_);
	}
}
