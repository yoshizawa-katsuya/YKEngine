#include "EnemySpawnEventTrigger.h"
#include "RailMover.h"

using namespace YKEngine;

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
