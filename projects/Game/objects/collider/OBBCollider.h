#pragma once
#include "BaseCollider.h"
#include "Struct.h"

/// <summary>
/// 方向付き境界ボックスのコライダークラス
/// 中心座標、座標軸、サイズを持ち、方向付き境界ボックスの衝突判定を行う。
/// </summary>
class OBBCollider : public BaseCollider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	const YKEngine::OBB& GetOBB() const { return obb_; }

private:

	YKEngine::OBB obb_;

};

