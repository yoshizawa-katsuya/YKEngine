#include "CuboidObstacle.h"

using namespace YKEngine;

void CuboidObstacle::Initialize(const WorldTransform& transform)
{
	OBBCollider::Initialize();

	// ワールド変換の初期化
	worldTransform_ = transform;

	typeID_ = CollisionTypeIdDef::kCuboidObstacle;

	// ワールド変換からOBBの中心座標、サイズ、座標軸を設定
	OBBCollider::Update();
}
