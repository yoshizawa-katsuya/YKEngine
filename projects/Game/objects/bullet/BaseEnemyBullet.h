#pragma once
#include "BaseBullet.h"
class Player;

/// <summary>
/// 敵弾クラス。
/// プレイヤーに当たるとダメージを与える。
/// </summary>
class BaseEnemyBullet : public BaseBullet
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期位置</param>
	/// <param name="velocity">初速度</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="target">ホーミングターゲット</param>
	/// <param name="speed">速さ</param>
	void Initialize(YKEngine::BaseModel* model, const YKEngine::Vector3& position, const YKEngine::Vector3& velocity, uint32_t textureHandle, Player* target, float speed);

	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="railCamera">レールカメラ。画面外に出たかの判定に使用。</param>
	void Update(YKEngine::Camera* railCamera);

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move() override; 

	/// <summary>
	/// ターゲットに向かって追尾する。
	/// </summary>
	void Homig();

	Player* target_ = nullptr;
	float speed_ = 0.5f;
	bool isHoming_ = true; // ホーミング弾かどうか
};