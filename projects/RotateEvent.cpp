#include "RotateEvent.h"
#include "RailMover.h"

void RotateEvent::Initialize(uint32_t waveNumber, const Vector3& position, const Vector3& rotate, float radius)
{
	Collider::Initialize();
	SetRadius(radius);
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	worldTransform_.UpdateMatrix();

	typeID_ = CollisionTypeIdDef::kRotateEvent;

	waveNumber_ = waveNumber;
}

void RotateEvent::OnCollision(Collider* other)
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
