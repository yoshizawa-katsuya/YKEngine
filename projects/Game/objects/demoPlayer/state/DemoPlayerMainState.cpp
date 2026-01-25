#include "DemoPlayerMainState.h"
#include "DemoPlayerStateContext.h"
#include "StateMachine.hpp"
#include "DemoPlayerEndState.h"

void DemoPlayerMainState::OnUpdate(DemoPlayerStateContext* demoPlayer)
{
	demoPlayer->UpdateMain();
	if (demoPlayer->IsSceneEnd())
	{
		stateMachine_->ChangeState<DemoPlayerEndState>();
	}
}
