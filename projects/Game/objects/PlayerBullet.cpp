#include "PlayerBullet.h"

void PlayerBullet::Update(float playerPositionX)
{
	float length = std::abs(worldTransform_.translation_.x - playerPositionX);

	if (length > 7.0f) {
		isAlive_ = false;
		return;
	}

	Bullet::Update();
}
