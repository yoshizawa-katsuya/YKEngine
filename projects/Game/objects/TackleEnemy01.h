#pragma once
#include "BaseEnemy.h"

/// <summary>
/// プレイヤーに向かって突進する敵キャラ。
/// 突進中はプレイヤーを追尾する。
/// </summary>
class TackleEnemy01 : public BaseEnemy
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TackleEnemy01() override;

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突したコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

	/// <summary>
	/// 接近時の更新処理。
	/// </summary>
	void UpdateApproach() override;

	/// <summary>
	/// メイン時の更新処理。
	/// </summary>
	void UpdateMain() override;

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
	/// 死亡時の処理。
	/// </summary>
	void Die(const YKEngine::Vector3& bulletVelocity) override;

	bool isHoming_ = true;

};

