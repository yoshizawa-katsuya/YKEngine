#include "GameSceneStartState.h"
#include "GameSceneStateContext.h"
#include "StateMachine.hpp"
#include "GameSceneMainState.h"

void GameSceneStartState::OnUpdate(GameSceneStateContext* gameScene)
{
	gameScene->UpdateStart();
	if (gameScene->StartCompleted())
	{
		stateMachine_->ChangeState<GameSceneMainState>();
	}
}
