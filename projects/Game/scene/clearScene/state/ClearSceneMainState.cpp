#include "ClearSceneMainState.h"
#include "Input.h"
#include "StateMachine.hpp"
#include "ClearSceneEndState.h"

using namespace YKEngine;

void ClearSceneMainState::OnEnter(ClearSceneStateContext* clearScene)
{
	input_ = Input::GetInstance();
}

void ClearSceneMainState::OnUpdate(ClearSceneStateContext* clearScene)
{
	//スペースキーまたはAボタンで終了
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A))
	{
		stateMachine_->ChangeState<ClearSceneEndState>();
	}
}

