#pragma once
#include "BaseEnemy.h"

/// <summary>
/// 自機に向かって突進してくる敵。
/// 追尾はしない。
/// </summary>
class TackleEnemy01 : public BaseEnemy
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="spawnData">出現データ</param>
	/// <param name="viewPortMatrix">ビューポート行列</param>
	/// <param name="railCamera">レールカメラ。画面内に入っているか判定するために使用。</param>
	void Initialize(YKEngine::BaseModel* model, const EnemySpawn& spawnData, YKEngine::Camera* railCamera, Player* player) override;

	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TackleEnemy01() override;

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突したコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

protected:

	/// <summary>
	/// メイン部の初期化。
	/// </summary>
	void MainInitialize() override;

	/// <summary>
	/// 接近時の更新処理。
	/// </summary>
	void UpdateApproach() override;

	/// <summary>
	/// メイン更新。
	/// </summary>
	void UpdateMain() override;

	/// <summary>
	/// 離脱時の更新処理。
	/// </summary>
	void UpdateLeave() override;

	/// <summary>
	/// 移動処理。
	/// </summary>
	virtual void Move() override;

	/// <summary>
	/// コライダーIDの設定。
	/// </summary>
	void SetColliderID() override;

	/// <summary>
	/// 死亡時の処理。
	/// </summary>
	void Die(const YKEngine::Vector3& bulletVelocity, const YKEngine::Vector3& bulletPosition) override;

	bool isHoming_ = true;

private:

	/// <summary>
	/// 回転。プレイヤーの方向を向く。
	/// </summary>
	void Rotate() override;

	YKEngine::WorldTransform target_;
	float targetRadius_; // ターゲットの半径

};

