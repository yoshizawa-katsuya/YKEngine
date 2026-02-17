#include "TitleSceneStartState.h"
#include "TitleSceneStateContext.h"
#include "StateMachine.hpp"
#include "TitleSceneMainState.h"

void TitleSceneStartState::OnUpdate(TitleSceneStateContext* titleScene)
{
	if (titleScene->IsSceneStagingEnd())
	{
		stateMachine_->ChangeState<TitleSceneMainState>();
	}
}
