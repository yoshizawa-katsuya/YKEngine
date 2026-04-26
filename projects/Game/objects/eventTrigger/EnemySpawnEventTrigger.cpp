#include "EnemySpawnEventTrigger.h"

using namespace YKEngine;

void EnemySpawnEventTrigger::Initialize(const YKEngine::ObjectData& objectData)
{
	BaseEventTrigger::Initialize(objectData);

	waveNumber_ = objectData.waveNum.value();
}
