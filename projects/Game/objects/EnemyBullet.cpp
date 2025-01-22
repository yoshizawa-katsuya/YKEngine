#include "EnemyBullet.h"
#include "ThreadPool.h"

void EnemyBullet::Update()
{
	ThreadPool::GetInstance()->enqueueTask([&]() {
		if (worldTransform_.translation_.y < radius_) {
			isAlive_ = false;
		}
		else if (worldTransform_.translation_.y > 7.0f) {
			isAlive_ = false;
		}

		Bullet::Update();
	});
}
