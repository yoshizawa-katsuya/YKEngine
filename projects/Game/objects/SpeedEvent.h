#pragma once
#include "Collider.h"

class SpeedEvent : public Collider
{
public:

	void Initialize(uint32_t waveNumber, const Vector3& position, float radius, float speed);

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	bool IsDead() { return isDead_; }

	uint32_t GetWaveNumber() const { return waveNumber_; }

	float GetSpeed() const { return speed_; }

private:

	uint32_t waveNumber_ = 0;
	float speed_ = 0.0f;

	bool isDead_ = false;
};

