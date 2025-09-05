#include "PlayerGroundState.h"
#include "Player.h"

void PlayerGroundState::Enter(BaseCharacter* character) {
	// characterをPlayerにキャストしてplayerに代入
	player = dynamic_cast<Player*>(character);

	// playerがnullptrだったら抜ける
	if (!player)
	{
		return;
	}
}

void PlayerGroundState::Update()
{
}

void PlayerGroundState::Exit()
{
}
