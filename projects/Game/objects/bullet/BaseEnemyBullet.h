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
	virtual void Initialize(YKEngine::BaseModel* model, const YKEngine::Vector3& position, const YKEngine::Vector3& velocity, Player* target, float speed);

	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="railCamera">レールカメラ。画面外に出たかの判定に使用。</param>
	virtual void Update(YKEngine::Camera* railCamera);

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] BaseCollider* other) override;

protected:

	float speed_ = 0.5f;
};