#include "RotateResetEvent.h"
#include "RailMover.h"

void RotateResetEvent::OnCollision(Collider* other)
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
