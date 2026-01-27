#include "GameSceneMainState.h"
#include "GameSceneStateContext.h"
#include "StateMachine.hpp"
#include "GameSceneClearState.h"
#include "GameSceneGameOverState.h"
#include "GameSceneTitleReturnState.h"
#include "GameScenePauseState.h"
#include "Input.h"

using namespace YKEngine;

void GameSceneMainState::OnUpdate(GameSceneStateContext* gameScene)
{
	gameScene->UpdateMain();
	//クリアしていたらクリアステートへ
	if (gameScene->CheckGameClear())
	{
		stateMachine_->ChangeState<GameSceneClearState>();
	}
	//ゲームオーバーしていたらゲームオーバーステートへ
	else if (gameScene->CheckGameOver())
	{
		stateMachine_->ChangeState<GameSceneGameOverState>();
	}
	else if (gameScene->IsReturnToTitle())
	{
		stateMachine_->ChangeState<GameSceneTitleReturnState>();
	}
	else if (Input::GetInstance()->TriggerKey(DIK_ESCAPE) || Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_START))
	{
		stateMachine_->ChangeState<GameScenePauseState>();
	}
}
