#include "SpeedEventTrigger.h"
#include "RailMover.h"

using namespace YKEngine;

void SpeedEventTrigger::Initialize(const YKEngine::ObjectData& objectData)
{
	BaseEventTrigger::Initialize(objectData);

	speed_ = objectData.speed.value();
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
