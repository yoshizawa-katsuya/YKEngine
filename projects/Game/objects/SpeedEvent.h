#pragma once
#include "BaseEvent.h"

class SpeedEvent : public BaseEvent
{
public:

	void Initialize(uint32_t waveNumber, const Vector3& position, float radius, float speed);

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	float GetSpeed() const { return speed_; }

private:

	float speed_ = 0.0f;
};

