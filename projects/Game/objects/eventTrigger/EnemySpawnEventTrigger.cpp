#include "EnemySpawnEventTrigger.h"
#include "RailMover.h"

void EnemySpawnEventTrigger::Initialize(uint32_t waveNumber, const Vector3& position, float radius)
{
	BaseEventTrigger::Initialize(waveNumber ,position, radius);
}

void EnemySpawnEventTrigger::OnCollision(Collider* other)
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
