#include "RailCameraGameOverState.h"
#include "RailCameraStateContext.h"

void RailCameraGameOverState::OnEnter(RailCameraStateContext* railCamera)
{
	railCamera->ResetT();
}

void RailCameraGameOverState::OnUpdate(RailCameraStateContext* railCamera)
{
	railCamera->UpdateGameOver();
}
