#include "PlayerDodgeState.h"
#include "PlayerStateContext.h"
#include "StateMachine.hpp"
#include "PlayerClearState.h"
#include "PlayerGameOverState.h"
#include "PlayerMainState.h"

using namespace YKEngine;

void PlayerDodgeState::OnEnter(PlayerStateContext* player)
{
	player->ResetT();

	Vector3 move = player->GetMove();
	if (move.x > 0.0f)
	{
		rotateSpeed_ *= -1.0f;
	}
	else if (move.x == 0.0f && move.y < 0.0f)
	{
		rotateSpeed_ *= -1.0f;
	}
}

void PlayerDodgeState::OnUpdate(PlayerStateContext* player)
{
	player->UpdateDodge();

	//回避中は回転する
	player->DodgeRotate(rotateSpeed_);

	//回避が終わったら通常状態へ
	if (!player->IsMoving())
	{
		stateMachine_->ChangeState<PlayerMainState>();
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
