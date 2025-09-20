#include "RotateResetEventTrigger.h"
#include "RailMover.h"

void RotateResetEventTrigger::OnCollision(Collider* other)
{
	if (RailMover* railMover = dynamic_cast<RailMover*>(other))
	{
		if (railMover->GetNextRotateResetWaveNumber() > waveNumber_)
		{
			isDead_ = true;
		}
		return;
	}

}
