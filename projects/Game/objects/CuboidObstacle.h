#pragma once
#include "OBBCollider.h"

/// <summary>
/// 直方体の障害物クラス
/// OBBColliderを継承し、OBBの衝突判定を行う。
/// </summary>
class CuboidObstacle : public OBBCollider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const YKEngine::WorldTransform& transform);

private:
};

