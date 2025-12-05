#include "SpeedEventTrigger.h"
#include "RailMover.h"

using namespace YKEngine;

void SpeedEventTrigger::Initialize(uint32_t waveNumber, const Vector3& position, float radius, float speed)
{
	BaseEventTrigger::Initialize(waveNumber, position, radius);

	speed_ = speed;
}

void SpeedEventTrigger::OnCollision(Collider* other)
{
	if (RailMover* railMover = dynamic_cast<RailMover*>(other))
	{
		if (railMover->GetNextSpeedWaveNumber() > waveNumber_)
		{
			isDead_ = true;
		}
		return;
	}
}
