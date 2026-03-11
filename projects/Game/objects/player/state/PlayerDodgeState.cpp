#include "PlayerDodgeState.h"
#include "PlayerStateContext.h"
#include "StateMachine.hpp"
#include "PlayerClearState.h"
#include "PlayerGameOverState.h"
#include "PlayerMainState.h"
#include "Easing.h"
#include "Lerp.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

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

	t_ += GlobalVariables::GetInstance()->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kDodgeLerpFactor); //ドッジの時間経過

	if (t_ > 1.0f)
	{
		t_ = 1.0f; //ドッジの時間が最大を超えないようにする
	}

	float rotateValue = Lerp(rotateSpeed_, 0.0f, EaseInCubic(t_)); //ドッジの回転速度を時間で減衰させる	

	//回避中は回転する
	player->DodgeRotate(rotateValue);

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
