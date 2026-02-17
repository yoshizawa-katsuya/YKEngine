#include "TitleSceneEndState.h"
#include "TitleSceneStateContext.h"

void TitleSceneEndState::OnEnter(TitleSceneStateContext* titleScene)
{
	titleScene->BeginSceneEndStaging();
}

void TitleSceneEndState::OnUpdate(TitleSceneStateContext* titleScene)
{
	titleScene->UpdateEnd();
}
