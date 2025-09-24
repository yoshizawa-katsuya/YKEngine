#pragma once
#include "BaseEventTrigger.h"

class EnemySpawnEventTrigger : public BaseEventTrigger
{
public:

	void Initialize(uint32_t waveNumber, const Vector3& position, float radius) override;

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

};

