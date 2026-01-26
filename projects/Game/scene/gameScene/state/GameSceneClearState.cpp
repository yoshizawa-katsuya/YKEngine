#include "GameSceneClearState.h"
#include "GameSceneStateContext.h"

void GameSceneClearState::OnEnter(GameSceneStateContext* gameScene)
{
	gameScene->ProcessGameClear();
}

void GameSceneClearState::OnUpdate(GameSceneStateContext* gameScene)
{
	gameScene->UpdateGameClear();
}
