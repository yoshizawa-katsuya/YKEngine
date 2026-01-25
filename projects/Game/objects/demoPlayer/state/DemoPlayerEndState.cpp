#include "DemoPlayerEndState.h"
#include "DemoPlayerStateContext.h"

void DemoPlayerEndState::OnEnter(DemoPlayerStateContext* demoPlayer)
{
	demoPlayer->BeforeEnd();
}

void DemoPlayerEndState::OnUpdate(DemoPlayerStateContext* demoPlayer)
{
	demoPlayer->UpdateEnd();
}
