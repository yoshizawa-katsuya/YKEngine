#pragma once
#include "WorldTransform.h"
#include "Rigid3dObject.h"
#include "RigidModel.h"
#include "Camera.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:

	virtual ~Collider() = default;

	//初期化
	void Initialize(RigidModel* model);

	//ワールドトランスフォームの更新
	void UpdateWorldTransform();
	//描画
	void Draw(Camera* camera);

	//衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]] Collider* other) {}

	//中心座標を取得
	virtual Vector3 GetCenterPosition() const = 0;

	//半径を取得
	float GetRadius() { return radius_; }

	void SetRadius(float radius) { radius_ = radius; }

	//種別IDを取得
	uint32_t GetTypeID() const { return typeID_; }

	//種別IDを設定
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; }

private:

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	std::unique_ptr<Rigid3dObject> object_;

	//衝突半径
	float radius_ = 1.5f;

	//種別ID
	uint32_t typeID_ = 0u;

};
