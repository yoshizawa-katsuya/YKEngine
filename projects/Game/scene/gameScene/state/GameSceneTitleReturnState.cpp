#include "GameSceneTitleReturnState.h"
#include "GameSceneStateContext.h"

void GameSceneTitleReturnState::OnEnter(GameSceneStateContext* gameScene)
{
	gameScene->StartSceneEndStaging({0.0f, 0.0f, 0.0f, 1.0f});
}

void GameSceneTitleReturnState::OnUpdate(GameSceneStateContext* gameScene)
{
	gameScene->UpdateTitleReturn();
}
