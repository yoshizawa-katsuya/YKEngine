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

