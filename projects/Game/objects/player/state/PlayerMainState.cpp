#include "PlayerMainState.h"
#include "PlayerStateContext.h"
#include "StateMachine.hpp"
#include "PlayerClearState.h"
#include "PlayerGameOverState.h"
#include "PlayerDodgeState.h"
#include "Input.h"

using namespace YKEngine;

void PlayerMainState::OnEnter(PlayerStateContext* player)
{
	input_ = Input::GetInstance();
}

void PlayerMainState::OnUpdate(PlayerStateContext* player)
{
	player->UpdateMain();
	
	if ((input_->TriggerKey(DIK_Q) || input_->TriggerButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) && player->IsMoving())
	{
		stateMachine_->ChangeState<PlayerDodgeState>();
	}
	
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
