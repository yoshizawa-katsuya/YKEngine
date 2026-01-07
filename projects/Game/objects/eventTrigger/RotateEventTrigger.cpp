#include "RotateEventTrigger.h"
#include "RailMover.h"

using namespace YKEngine;

void RotateEventTrigger::Initialize(const YKEngine::ObjectData& objectData)
{
	BaseEventTrigger::Initialize(objectData);

	worldTransform_.rotation_ = objectData.transform.rotation;
	worldTransform_.UpdateMatrix();
}

void RotateEventTrigger::OnCollision(Collider* other)
{
	if (RailMover* railMover = dynamic_cast<RailMover*>(other))
	{
		if (railMover->GetNextRotateWaveNumber() > waveNumber_)
		{
			isDead_ = true;
		}
		return;
	}
}
