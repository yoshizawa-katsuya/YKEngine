#include "ClearSceneStartState.h"
#include "ClearSceneStateContext.h"
#include "StateMachine.hpp"
#include "ClearSceneMainState.h"

void ClearSceneStartState::OnUpdate(ClearSceneStateContext* clearScene)
{
	if (clearScene->IsEndSceneChangeStaging())
	{
		stateMachine_->ChangeState<ClearSceneMainState>();
	}
}
