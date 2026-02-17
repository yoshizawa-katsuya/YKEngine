#include "TitleSceneMainState.h"
#include "Input.h"
#include "StateMachine.hpp"
#include "TitleSceneEndState.h"

using namespace YKEngine;

TitleSceneMainState::TitleSceneMainState()
	:input_(Input::GetInstance())
{
}

void TitleSceneMainState::OnUpdate(TitleSceneStateContext* titleScene)
{
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A))
	{
		stateMachine_->ChangeState<TitleSceneEndState>();
	}
}
