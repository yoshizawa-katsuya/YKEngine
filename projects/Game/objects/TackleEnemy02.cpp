#include "TackleEnemy02.h"

using namespace YKEngine;

void TackleEnemy02::Move()
{
	// 移動
	// ホーミング移動
	if (isHoming_)
	{
		Vector3 normalDirection = Normalize(direction_);
		velocity_ = normalDirection * speed_;
		worldTransform_.translation_ += velocity_;
	}
	else
	{
		BaseEnemy::Move();
	}
}
