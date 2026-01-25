#include "PlayerGameOverState.h"
#include "PlayerStateContext.h"

void PlayerGameOverState::OnEnter(PlayerStateContext* player)
{
	player->GameOverRotate();
}

void PlayerGameOverState::OnUpdate(PlayerStateContext* player)
{
	player->UpdateGameOver();
}
