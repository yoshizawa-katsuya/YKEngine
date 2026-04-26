#include "RotateEventTrigger.h"

using namespace YKEngine;

void RotateEventTrigger::Initialize(const YKEngine::ObjectData& objectData)
{
	BaseEventTrigger::Initialize(objectData);

	worldTransform_.rotation_ = objectData.transform.rotation;
	worldTransform_.UpdateMatrix();
}
