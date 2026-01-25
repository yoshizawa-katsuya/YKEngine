#include "PlayerStartState.h"
#include "PlayerStateContext.h"
#include "StateMachine.hpp"
#include "PlayerMainState.h"

void PlayerStartState::OnUpdate(PlayerStateContext* player)
{
	player->UpdateStart();
	if (player->StartCompleted())
	{
		stateMachine_->ChangeState<PlayerMainState>();
	}
}

void PlayerStartState::OnExit(PlayerStateContext* player)
{
	player->AfterStartComplete();
}
