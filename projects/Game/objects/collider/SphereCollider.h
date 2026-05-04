#pragma once
#include "BaseCollider.h"

class SphereCollider : public BaseCollider
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	virtual ~SphereCollider() override = default;

	/// <summary>
	/// 初期化。
	/// </summary>
	virtual void Initialize() override;

	//半径を取得
	float GetRadius() { return radius_; }

	void SetRadius(float radius);

protected:

	//衝突半径
	float radius_ = 1.0f;

};

