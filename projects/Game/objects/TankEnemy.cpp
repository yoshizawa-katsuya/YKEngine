#include "TankEnemy.h"

void TankEnemy::Move()
{
	velocity_ = YKEngine::Normalize(velocity_) * maxSpeed_;
}
