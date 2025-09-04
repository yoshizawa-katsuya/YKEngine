#include "PlayerIdleState.h"

#include "Player.h"

void PlayerIdleState::Enter(BaseCharacter* character)
{
	player = static_cast<Player*>(character);

	if (!player)
	{
		return;
	}
}

void PlayerIdleState::Update()
{
}

void PlayerIdleState::Exit()
{
}
