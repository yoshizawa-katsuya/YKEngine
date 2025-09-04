#include "PlayerJumpState.h"
#include "Player.h"

void PlayerJumpState::Enter(BaseCharacter* character)
{
	// characterをPlayerにキャストしてplayerに代入
	player = dynamic_cast<Player*>(character);

	// playerがnullptrだったら抜ける
	if (!player)
	{
		return;
	}
}

void PlayerJumpState::Update()
{
	Vector3& velocity = player->GetVelocity();

	BaseCharacter::PhysicsParam physicsParam_ = player->GetPhysicsParam();

	// 落下速度
	velocity.y += -physicsParam_.kGravityAcceleration;
	// 落下速度制限	
	velocity.y = (std::max)(velocity.y, -physicsParam_.kLimitFallSpeed);
}

void PlayerJumpState::Exit()
{
}
