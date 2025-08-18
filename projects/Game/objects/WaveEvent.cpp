#include "WaveEvent.h"
#include "RailMover.h"

void WaveEvent::Initialize(uint32_t waveNumber, const Vector3& position, float radius)
{
	BaseEvent::Initialize(waveNumber ,position, radius);
}

void WaveEvent::OnCollision(Collider* other)
{
	if (RailMover* railMover = dynamic_cast<RailMover*>(other))
	{
		if (railMover->GetNextEnemyWaveNumber() > waveNumber_)
		{
			isDead_ = true;
		}
		return;
	}
	
}
