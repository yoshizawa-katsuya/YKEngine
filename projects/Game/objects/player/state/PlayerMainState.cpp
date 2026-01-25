#include "PlayerMainState.h"
#include "PlayerStateContext.h"
#include "StateMachine.hpp"
#include "PlayerClearState.h"
#include "PlayerGameOverState.h"

void PlayerMainState::OnUpdate(PlayerStateContext* player)
{
	player->UpdateMain();
	
	//クリアしたら
	if (player->GetIsGameClear())
	{
		stateMachine_->ChangeState<PlayerClearState>();
	}
	//ゲームオーバーになったら
	else if (player->IsDead())
	{
		stateMachine_->ChangeState<PlayerGameOverState>();
	}
}
