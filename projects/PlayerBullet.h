#pragma once
#include "WorldTransform.h"
#include "BaseModel.h"
class PlayerBullet
{
public:
	/// <summary>
	///  デストラクタ
	/// </summary>
	~PlayerBullet();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera* camera);

	bool IsDead() const { return isDead_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	BaseModel* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};

