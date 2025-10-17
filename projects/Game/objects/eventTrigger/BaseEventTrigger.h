#pragma once
#include "Collider.h"

/// <summary>
/// イベントトリガーの基底クラス。
/// RailMoverが通過したときにイベントを発生させる。
/// </summary>
class BaseEventTrigger : public Collider
{
public:

	virtual void Initialize(uint32_t waveNumber, const Vector3& position, float radius);

	bool IsDead() { return isDead_; }

	uint32_t GetWaveNumber() const { return waveNumber_; }

protected:

	uint32_t waveNumber_ = 0;

	bool isDead_ = false;
};

