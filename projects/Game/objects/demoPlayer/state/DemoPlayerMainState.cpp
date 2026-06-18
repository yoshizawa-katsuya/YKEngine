#include "DemoPlayerMainState.h"
#include "DemoPlayerStateContext.h"
#include "StateMachine.hpp"
#include "DemoPlayerEndState.h"
#include "manager/AudioManager.h"

void DemoPlayerMainState::OnEnter(DemoPlayerStateContext* demoPlayer)
{
	// デモプレイヤーの出現SEを再生
	AudioManager::GetInstance()->PlaySE(SEType::kAppear03);
}

void DemoPlayerMainState::OnUpdate(DemoPlayerStateContext* demoPlayer)
{
	demoPlayer->UpdateMain();
	if (demoPlayer->IsSceneEnd())
	{
		stateMachine_->ChangeState<DemoPlayerEndState>();
	}
}
