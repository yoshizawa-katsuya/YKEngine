#pragma once
#include "BaseModel.h"
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Vector.h"

class Camera;

class BossBullet {
public:

	void Initialize(Base3dObject* object, const Vector3& translation, const Vector3& velocity);

	void Update(Camera* camera);

	void Draw();

	void OnCollision();

	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	float GetRadius()const { return radius_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

private:
	std::unique_ptr<BaseModel> model_;
	Base3dObject* object_;
	WorldTransform worldTransform_;

	Vector3 velocity_;

	// 寿命＜frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

	// 当たり判定の大きさ（半径）
	float radius_ = 0.8f;

};
