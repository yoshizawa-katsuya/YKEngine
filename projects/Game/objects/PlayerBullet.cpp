#include "PlayerBullet.h"
#include "ThreadPool.h"

void PlayerBullet::Update(float playerPositionX)
{
	ThreadPool::GetInstance()->enqueueTask([&, playerPositionX]() {
		float length = std::abs(worldTransform_.translation_.x - playerPositionX);

		if (length > 7.0f) {
			isAlive_ = false;
			return;
		}

		Bullet::Update();
	});
}
