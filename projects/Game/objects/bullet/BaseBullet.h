#pragma once
#include "BaseCharacter.h"

class BaseBullet : public BaseCharacter
{
public:

	// 初期化
	virtual void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle);

	// 更新
	virtual void Update() override;

	//衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]] Collider* other) override = 0;

	// 描画
	virtual void Draw(Camera* camera);

	virtual int32_t GetAttackPower() { return attackPower_; }

private:

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

protected:

	//攻撃力
	int32_t attackPower_ = 1;

};

