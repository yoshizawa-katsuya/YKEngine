#include "EnemyLeaveState.h"
#include "EnemyStateContext.h"
#include "EnemyMainState.h"
#include "StateMachine.hpp"

void EnemyLeaveState::OnEnter(EnemyStateContext* enemy)
{
	enemy->LeaveInitialize();
}

void EnemyLeaveState::OnUpdate(EnemyStateContext* enemy)
{
	enemy->UpdateLeave();

	//画面内に戻ってきたらメインフェーズへ。画面の揺れなどで戻ってきた場合を考慮し、離脱タイマーもリセットする
	if (enemy->IsMain())
	{
		stateMachine_->ChangeState<EnemyMainState>();
	}
}
