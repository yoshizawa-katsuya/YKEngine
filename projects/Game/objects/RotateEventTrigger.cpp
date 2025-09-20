#include "RotateEventTrigger.h"
#include "RailMover.h"

void RotateEventTrigger::Initialize(uint32_t waveNumber, const Vector3& position, const Vector3& rotate, float radius)
{
	BaseEventTrigger::Initialize(waveNumber, position, radius);

	worldTransform_.rotation_ = rotate;
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
