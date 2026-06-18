#include "EnemySpawnObjectStartState.h"
#include "EnemySpawnObjectStateContext.h"
#include "EnemySpawnObjectIntervalState.h"
#include "StateMachine.hpp"
#include "manager/AudioManager.h"

void EnemySpawnObjectStartState::OnEnter(EnemySpawnObjectStateContext* enemySpawnObject)
{
	// 敵出現開始SEを流す
	AudioManager::GetInstance()->PlaySE(SEType::kStart02);
}

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
