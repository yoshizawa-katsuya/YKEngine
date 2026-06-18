#include "GameOverSceneMainState.h"
#include "Input.h"
#include "StateMachine.hpp"
#include "GameOverSceneEndState.h"
#include "manager/AudioManager.h"

using namespace YKEngine;

GameOverSceneMainState::GameOverSceneMainState()
	: input_(Input::GetInstance())
{
}

void GameOverSceneMainState::OnUpdate(GameOverSceneStateContext* gameOverScene)
{
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A))
	{
		stateMachine_->ChangeState<GameOverSceneEndState>();
	}
}

void GameOverSceneMainState::OnExit(GameOverSceneStateContext* gameOverScene)
{
	//決定SEを流す
	AudioManager::GetInstance()->PlaySE(SEType::kDecision01);
}
