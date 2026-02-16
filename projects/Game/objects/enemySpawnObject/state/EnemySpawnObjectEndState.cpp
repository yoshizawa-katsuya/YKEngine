#include "EnemySpawnObjectEndState.h"
#include "EnemySpawnObjectStateContext.h"

void EnemySpawnObjectEndState::OnUpdate(EnemySpawnObjectStateContext* enemySpawnObject)
{
	enemySpawnObject->UpdateWaveEnd();
}
