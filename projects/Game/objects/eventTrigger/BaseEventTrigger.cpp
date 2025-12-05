#include "BaseEventTrigger.h"

using namespace YKEngine;

void BaseEventTrigger::Initialize(uint32_t waveNumber, const Vector3& position, float radius)
{
	Collider::Initialize();
	SetRadius(radius);
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();

	typeID_ = CollisionTypeIdDef::kEvent;

	waveNumber_ = waveNumber;
}
