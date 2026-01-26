#include "GameSceneGameOverState.h"
#include "GameSceneStateContext.h"

void GameSceneGameOverState::OnEnter(GameSceneStateContext* gameScene)
{
	gameScene->ProcessGameOver();
}

void GameSceneGameOverState::OnUpdate(GameSceneStateContext* gameScene)
{
	gameScene->UpdateGameOver();
}
