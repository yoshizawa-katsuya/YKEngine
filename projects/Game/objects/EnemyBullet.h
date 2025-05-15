#pragma once
#include "BaseCharacter.h"

class EnemyBullet : public BaseCharacter
{
public:

	//初期化
	void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle);

	void Update() override;

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	// 描画
	void Draw(Camera* camera);

	bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	const float GetRadius() const { return radius_; }

private:

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;

};