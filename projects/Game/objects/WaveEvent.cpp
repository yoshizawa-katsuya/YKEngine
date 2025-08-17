#include "WaveEvent.h"
#include "RailMover.h"

void WaveEvent::Initialize(uint32_t waveNumber, const Vector3& position, float radius)
{
	Collider::Initialize();
	SetRadius(radius);
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();

	typeID_ = CollisionTypeIdDef::kWaveEvent;

	waveNumber_ = waveNumber;
}

void WaveEvent::OnCollision(Collider* other)
{
	if (RailMover* railMover = dynamic_cast<RailMover*>(other))
	{
		if (railMover->GetNextWaveNumber() > waveNumber_)
		{
			isDead_ = true;
		}
		return;
	}
	
}
