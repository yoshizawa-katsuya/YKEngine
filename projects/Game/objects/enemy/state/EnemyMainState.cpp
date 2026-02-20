#include "EnemyMainState.h"
#include "EnemyStateContext.h"
#include "StateMachine.hpp"
#include "EnemyLeaveState.h"
#include "EnemyDeadState.h"

void EnemyMainState::OnEnter(EnemyStateContext* enemy)
{
	enemy->MainInitialize();
}

void EnemyMainState::OnUpdate(EnemyStateContext* enemy)
{
	enemy->UpdateMain();

	//レールカメラに映っていなかったら離脱フェーズへ
	if (enemy->IsLeave())
	{
		stateMachine_->ChangeState<EnemyLeaveState>();
	}
	else if (enemy->IsDeadStatus())
	{
		stateMachine_->ChangeState<EnemyDeadState>();
	}
}
