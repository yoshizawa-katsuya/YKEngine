#include "GameOverSceneEndState.h"
#include "GameOverSceneStateContext.h"

void GameOverSceneEndState::OnEnter(GameOverSceneStateContext* gameOverScene)
{
	gameOverScene->BeginSceneEndStaging();
}

void GameOverSceneEndState::OnUpdate(GameOverSceneStateContext* gameOverScene)
{
	gameOverScene->UpdateEnd();
}
