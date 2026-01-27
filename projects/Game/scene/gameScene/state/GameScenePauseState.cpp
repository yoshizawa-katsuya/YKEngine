#include "GameScenePauseState.h"
#include "GameSceneStateContext.h"
#include "StateMachine.hpp"
#include "Input.h"
#include "GameSceneMainState.h"
#include "GameSceneTitleReturnState.h"
#include "Pause.h"

using namespace YKEngine;

void GameScenePauseState::OnEnter(GameSceneStateContext* gameScene)
{
	gameScene->ProcessPause();
}

void GameScenePauseState::OnUpdate(GameSceneStateContext* gameScene)
{
	gameScene->UpdatePause();
	Pause* pause = gameScene->GetPause();

	Input* input = Input::GetInstance();
	if (!pause->GetIsPause())
	{
		stateMachine_->ChangeState<GameSceneMainState>();
	}
	else if (pause->GetIsReturnToTitle())
	{
		stateMachine_->ChangeState<GameSceneTitleReturnState>();
	}
}

