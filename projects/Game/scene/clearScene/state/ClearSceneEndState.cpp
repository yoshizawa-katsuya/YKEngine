#include "ClearSceneEndState.h"
#include "ClearSceneStateContext.h"

void ClearSceneEndState::OnEnter(ClearSceneStateContext* clearScene)
{
	clearScene->EnterEnd();
}

void ClearSceneEndState::OnUpdate(ClearSceneStateContext* clearScene)
{
	clearScene->UpdateEnd();
}
