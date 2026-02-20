#include "EnemyDeadState.h"
#include "EnemyStateContext.h"

void EnemyDeadState::OnEnter(EnemyStateContext* enemy)
{
	enemy->DeadInitialize();
}

void EnemyDeadState::OnUpdate(EnemyStateContext* enemy)
{
	enemy->UpdateDead();
}
