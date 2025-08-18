#include "BaseEvent.h"

void BaseEvent::Initialize(uint32_t waveNumber, const Vector3& position, float radius)
{
	Collider::Initialize();
	SetRadius(radius);
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();

	typeID_ = CollisionTypeIdDef::kEvent;

	waveNumber_ = waveNumber;
}
