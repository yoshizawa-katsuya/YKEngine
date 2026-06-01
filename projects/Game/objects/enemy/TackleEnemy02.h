#pragma once
#include "BaseEnemy.h"

/// <summary>
/// プレイヤーに向かって突進する敵キャラ。
/// 突進中はプレイヤーを追尾する。
/// </summary>
class TackleEnemy02 : public BaseEnemy
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TackleEnemy02() override;

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="animation">アニメーション</param>
	/// <param name="spawnData">出現データ</param>
	/// <param name="viewPortMatrix">ビューポート行列</param>
	/// <param name="railCamera">レールカメラ。画面内に入っているか判定するために使用。</param>
	void Initialize(YKEngine::BaseModel* model, YKEngine::Animation* animation, const EnemySpawn& spawnData, YKEngine::Camera* railCamera, Player* player) override;

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突したコライダー</param>
	void OnCollision([[maybe_unused]] BaseCollider* other) override;

private:

	/// <summary>
	/// 接近時の更新処理。
	/// </summary>
	void UpdateApproach() override;

	/// <summary>
	/// 離脱時の更新処理。
	/// </summary>
	void UpdateLeave() override;

	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move() override;

	/// <summary>
	/// コライダーIDの設定。
	/// </summary>
	void SetColliderID() override;

	/// <summary>
	/// 死亡部の初期化。
	/// </summary>
	void DeadInitialize() override;

	bool isHoming_ = true;

};

