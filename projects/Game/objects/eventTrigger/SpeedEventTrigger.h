#pragma once
#include "BaseEventTrigger.h"

/// <summary>
/// 速度イベントトリガー。
/// RailMoverの速度を変更するイベントトリガー。
/// </summary>
class SpeedEventTrigger : public BaseEventTrigger
{
public:

	void Initialize(uint32_t waveNumber, const Vector3& position, float radius, float speed);

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	float GetSpeed() const { return speed_; }

private:

	float speed_ = 0.0f;
};

