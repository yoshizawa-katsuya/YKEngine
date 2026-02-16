#include "RailCameraMainState.h"
#include "StateMachine.hpp"
#include "RailCameraGameOverState.h"
#include "RailCameraStateContext.h"

void RailCameraMainState::OnUpdate(RailCameraStateContext* railCamera)
{
	railCamera->UpdateMain();
	if (railCamera->IsGameOver())
	{
		stateMachine_->ChangeState<RailCameraGameOverState>();
	}
}
