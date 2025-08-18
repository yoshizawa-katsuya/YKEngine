#include "SpeedEvent.h"
#include "RailMover.h"

void SpeedEvent::Initialize(uint32_t waveNumber, const Vector3& position, float radius, float speed)
{
	BaseEvent::Initialize(waveNumber, position, radius);

	speed_ = speed;
}

void SpeedEvent::OnCollision(Collider* other)
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
