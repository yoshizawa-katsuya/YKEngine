#pragma once
#include "BaseEventTrigger.h"

/// <summary>
/// 敵出現イベントトリガー。
/// RailMoverが一定範囲に入ると敵が出現する。
/// </summary>
class EnemySpawnEventTrigger : public BaseEventTrigger
{
public:

	void Initialize(uint32_t waveNumber, const Vector3& position, float radius) override;

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

};

