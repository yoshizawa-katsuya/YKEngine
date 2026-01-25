#include "PlayerClearState.h"
#include "PlayerStateContext.h"

void PlayerClearState::OnUpdate(PlayerStateContext* player)
{
	player->UpdateGameClear();
}
