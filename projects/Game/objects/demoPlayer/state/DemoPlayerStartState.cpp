#include "DemoPlayerStartState.h"
#include "DemoPlayerStateContext.h"
#include "StateMachine.hpp"
#include "DemoPlayerMainState.h"

void DemoPlayerStartState::OnUpdate(DemoPlayerStateContext* demoPlayer)
{
	demoPlayer->UpdateStart();
	if (demoPlayer->IsEndAnimation())
	{
		stateMachine_->ChangeState<DemoPlayerMainState>();
	}
}

void DemoPlayerStartState::OnExit(DemoPlayerStateContext* demoPlayer)
{
	demoPlayer->AfterStartComplete();
}
