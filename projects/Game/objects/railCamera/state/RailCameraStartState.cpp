#include "RailCameraStartState.h"
#include "StateMachine.hpp"
#include "RailCameraStateContext.h"
#include "RailCameraMainState.h"

void RailCameraStartState::OnEnter(RailCameraStateContext* railCamera)
{
	railCamera->ResetT();
}

void RailCameraStartState::OnUpdate(RailCameraStateContext* railCamera)
{
	railCamera->UpdateStart();
	if (railCamera->GetT() >= 1.0f) {
		stateMachine_->ChangeState<RailCameraMainState>();
	}
}
