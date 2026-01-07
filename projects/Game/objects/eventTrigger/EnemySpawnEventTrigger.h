#pragma once
#include "BaseEventTrigger.h"

/// <summary>
/// 敵出現イベントトリガー。
/// RailMoverが一定範囲に入ると敵が出現する。
/// </summary>
class EnemySpawnEventTrigger : public BaseEventTrigger
{
public:

	/// <summary>
	/// 衝突時処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

};

