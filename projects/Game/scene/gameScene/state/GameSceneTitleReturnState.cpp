#include "GameSceneTitleReturnState.h"
#include "GameSceneStateContext.h"

void GameSceneTitleReturnState::OnEnter(GameSceneStateContext* gameScene)
{
	gameScene->StartSceneEndStaging();
}

void GameSceneTitleReturnState::OnUpdate(GameSceneStateContext* gameScene)
{
	gameScene->UpdateTitleReturn();
}
