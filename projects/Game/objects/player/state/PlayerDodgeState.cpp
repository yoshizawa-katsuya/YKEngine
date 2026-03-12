#include "PlayerDodgeState.h"
#include "PlayerStateContext.h"
#include "StateMachine.hpp"
#include "PlayerClearState.h"
#include "PlayerGameOverState.h"
#include "PlayerMainState.h"
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
		rotateQuantity_ *= -1.0f;
	}
	else if (move.x == 0.0f && move.y < 0.0f)
	{
		rotateQuantity_ *= -1.0f;
	}

	//回避アニメーションの設定
	const float kAnimeDuration = GlobalVariables::GetInstance()->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kDodgeTime) / 60.0f;	//フレーム数を秒数に変換
	rotateAnime_ = std::make_unique<SRTAnimator>();
	rotateAnime_->SetAnimation({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, rotateQuantity_ }, kAnimeDuration);
	rotateAnime_->SetEasingType(EasingType::kEaseOutCubic);
}

void PlayerDodgeState::OnUpdate(PlayerStateContext* player)
{
	player->UpdateDodge();

	//回避中は回転する
	player->DodgeRotate(rotateAnime_.get());

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
