#include "EnemyApproachState.h"
#include "EnemyStateContext.h"
#include "StateMachine.hpp"
#include "EnemyMainState.h"

void EnemyApproachState::OnUpdate(EnemyStateContext* enemy)
{
	enemy->UpdateApproach();

	//レールカメラに映っていたらメインフェーズへ
	if (enemy->IsInRailCamera())
	{
		stateMachine_->ChangeState<EnemyMainState>();
	}
}
