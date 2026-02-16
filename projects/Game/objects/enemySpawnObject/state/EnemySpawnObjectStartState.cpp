#include "EnemySpawnObjectStartState.h"
#include "EnemySpawnObjectStateContext.h"
#include "EnemySpawnObjectIntervalState.h"
#include "StateMachine.hpp"

void EnemySpawnObjectStartState::OnUpdate(EnemySpawnObjectStateContext* enemySpawnObject)
{
	enemySpawnObject->UpdateWaveStart();
	if (enemySpawnObject->IsWaveStartEnd())
	{
		stateMachine_->ChangeState<EnemySpawnObjectIntervalState>();
	}
}

void EnemySpawnObjectStartState::OnExit(EnemySpawnObjectStateContext* enemySpawnObject)
{
	//タイマーをリセットする
	enemySpawnObject->ResetTimer();
}
