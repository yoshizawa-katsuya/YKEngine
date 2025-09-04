#include "PlayerWalkState.h"
#include "Player.h"

void PlayerWalkState::Enter(BaseCharacter* character)
{
	player = static_cast<Player*>(character);

	if (!player)
	{
		return;
	}
}

void PlayerWalkState::Update()
{
}

void PlayerWalkState::Exit()
{
}
