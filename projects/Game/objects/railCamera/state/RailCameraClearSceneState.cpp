#include "RailCameraClearSceneState.h"

void RailCameraClearSceneState::OnEnter(RailCameraStateContext* railCamera)
{
	railCamera->EnterClearScene();
}

void RailCameraClearSceneState::OnUpdate(RailCameraStateContext* railCamera)
{
	railCamera->UpdateClearScene();
}
