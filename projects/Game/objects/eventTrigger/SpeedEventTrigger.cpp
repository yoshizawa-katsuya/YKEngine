#include "SpeedEventTrigger.h"

using namespace YKEngine;

void SpeedEventTrigger::Initialize(const YKEngine::ObjectData& objectData)
{
	BaseEventTrigger::Initialize(objectData);

	speed_ = objectData.speed.value();
}
