#include "RailCameraGameOverState.h"

void RailCameraGameOverState::OnEnter(RailCameraStateContext* railCamera)
{
	railCamera->ReaetT();
}

void RailCameraGameOverState::OnUpdate(RailCameraStateContext* railCamera)
{
	railCamera->UpdateGameOver();
}
