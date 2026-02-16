#include "EnemySpawnObjectIntervalState.h"
#include "EnemySpawnObjectStateContext.h"
#include "EnemySpawnObjectEndState.h"
#include "StateMachine.hpp"

void EnemySpawnObjectIntervalState::OnUpdate(EnemySpawnObjectStateContext* enemySpawnObject)
{
	enemySpawnObject->UpdateWaveInterval();
	if (enemySpawnObject->IsWaveIntervalEnd()) 
	{
		stateMachine_->ChangeState<EnemySpawnObjectEndState>();
	}
}

void EnemySpawnObjectIntervalState::OnExit(EnemySpawnObjectStateContext* enemySpawnObject)
{
	enemySpawnObject->SetTimerToSwitchTime();
	enemySpawnObject->SpawnEnemies();
}
