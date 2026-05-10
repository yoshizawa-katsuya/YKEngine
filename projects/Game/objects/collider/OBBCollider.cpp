#include "OBBCollider.h"

void OBBCollider::Initialize()
{
	BaseCollider::Initialize();

	shapeType_ = ColliderShapeType::kSphere;
}

void OBBCollider::Update()
{
	BaseCollider::Update();

	// ワールド変換からOBBの中心座標、サイズ、座標軸を設定
	obb_.center = worldTransform_.GetWorldPosition();
	obb_.size = worldTransform_.scale_;
	// ワールド行列の1列目、2列目、3列目がそれぞれOBBの座標軸になる
	for (int i = 0; i < 3; i++)
	{
		// ワールド行列のi列目をOBBの座標軸に設定
		obb_.orientations[i] = { worldTransform_.worldMatrix_.m[0][i], worldTransform_.worldMatrix_.m[1][i], worldTransform_.worldMatrix_.m[2][i] };
		obb_.orientations[i] = Normalize(obb_.orientations[i]);
	}
		
	
}
