#pragma once
#include "Collider.h"

class WaveEvent : public Collider
{
public:

	void Initialize(uint32_t waveNumber, const Vector3& position, float radius);

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	bool IsDead() { return isDead_; }

	uint32_t GetWaveNumber() const { return waveNumber_; }

private:

	uint32_t waveNumber_ = 0;

	bool isDead_ = false;

};

