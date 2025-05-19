#pragma once
#include "WorldTransform.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider
{
public:

	virtual ~Collider() = default;

	//初期化
	void Initialize();

	void Update();

	//衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]] Collider* other) {}

	//中心座標を取得
	virtual Vector3 GetCenterPosition();

	//半径を取得
	float GetRadius() { return radius_; }

	void SetRadius(float radius);

	//種別IDを取得
	uint32_t GetTypeID() const { return typeID_; }

	//種別IDを設定
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

protected:

	//Transform変数を作る
	WorldTransform worldTransform_;

	//衝突半径
	float radius_ = 1.0f;

	//種別ID
	uint32_t typeID_ = 0u;
};

