#pragma once
#include "BaseCharacter.h"

/// <summary>
/// 弾の基底クラス。
/// 継承して使用する。
/// </summary>
class BaseBullet : public BaseCharacter
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期位置</param>
	/// <param name="velocity">初速度</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	virtual void Initialize(YKEngine::BaseModel* model, const YKEngine::Vector3& position, const YKEngine::Vector3& velocity, uint32_t textureHandle);

	/// <summary>
	/// 更新。
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 衝突時の処理。純粋仮想関数。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	virtual void OnCollision([[maybe_unused]] Collider* other) override = 0;

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">描画に使用するカメラ</param>
	virtual void Draw(YKEngine::Camera* camera);

	virtual int32_t GetAttackPower() { return attackPower_; }

	YKEngine::Vector3 GetVelocity() { return velocity_; }

private:

	/// <summary>
	/// 移動処理。
	/// </summary>
	virtual void Move();

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	

	//寿命
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

protected:
	//速度
	YKEngine::Vector3 velocity_;

	//攻撃力
	int32_t attackPower_ = 1;

};

