#include "PlayerJumpState.h"
#include "Player.h"

void PlayerJumpState::Enter(BaseCharacter* character)
{
	// characterをPlayerにキャストしてplayerに代入
	player = static_cast<Player*>(character);

	// playerがnullptrだったら抜ける
	if (!player)
	{
		return;
	}

	// 
	Vector3 velocity = player->GetVelocity();
}

void PlayerJumpState::Update()
{
}

void PlayerJumpState::Exit()
{
}
