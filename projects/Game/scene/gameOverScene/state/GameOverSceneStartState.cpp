#include "GameOverSceneStartState.h"
#include "GameOverSceneStateContext.h"
#include "StateMachine.hpp"
#include "GameOverSceneMainState.h"

void GameOverSceneStartState::OnUpdate(GameOverSceneStateContext* gameOverScene)
{
	if (gameOverScene->IsSceneStagingEnd())
	{
		stateMachine_->ChangeState<GameOverSceneMainState>();
	}
}
