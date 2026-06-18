#include "PlayerStartState.h"
#include "PlayerStateContext.h"
#include "StateMachine.hpp"
#include "PlayerMainState.h"
#include "manager/AudioManager.h"

void PlayerStartState::OnEnter(PlayerStateContext* player)
{
	AudioManager::GetInstance()->PlaySE(SEType::kStart01);
}

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
	//開始部の処理が完了した後の処理
	player->AfterStartComplete();

	//出現SE再生
	AudioManager::GetInstance()->PlaySE(SEType::kAppear01);
}
