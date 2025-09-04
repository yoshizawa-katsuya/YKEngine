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

	// 
	velocity_ = player->GetVelocity();

	BaseCharacter::PhysicsParam physicsParam_ = player->GetPhysicsParam();

	velocity_.y += physicsParam_.kJumpAcceleration;
}

void PlayerJumpState::Update()
{
	BaseCharacter::PhysicsParam physicsParam_ = player->GetPhysicsParam();

	// 落下速度
	velocity_.y += -physicsParam_.kGravityAcceleration;
	// 落下速度制限	
	velocity_.y = (std::max)(velocity_.y, -physicsParam_.kLimitFallSpeed);
}

void PlayerJumpState::Exit()
{
}
