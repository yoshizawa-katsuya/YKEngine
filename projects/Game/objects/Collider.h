#pragma once
#include "WorldTransform.h"
#include "CollisionTypeIdDef.h"
class Camera;

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

	//画面に表示されているかどうかを取得
	virtual bool IsVisible(Camera* camera);

	//中心座標を取得
	virtual Vector3 GetCenterPosition();

	//半径を取得
	float GetRadius() { return radius_; }

	void SetRadius(float radius);

	//種別IDを取得
	CollisionTypeIdDef GetTypeID() const { return typeID_; }

	//種別IDを設定
	void SetTypeID(CollisionTypeIdDef typeID) { typeID_ = typeID; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

protected:

	//Transform変数を作る
	WorldTransform worldTransform_;

	//衝突半径
	float radius_ = 1.0f;

	//種別ID
	CollisionTypeIdDef typeID_ = CollisionTypeIdDef::kDefault;
};

