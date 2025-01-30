#pragma once
#include <Vector3.h>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:

	virtual ~Collider() = default;

	//初期化
	void Initialize();

	//ワールドトランスフォームの更新
	void UpdateWorldTransform();
	//描画
	void Draw(Model* model, const ViewProjection& viewProjection);

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

	//衝突半径
	float radius_ = 1.5f;

	//種別ID
	uint32_t typeID_ = 0u;

};
