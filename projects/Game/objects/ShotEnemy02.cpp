#include "ShotEnemy02.h"

void ShotEnemy02::Initialize(YKEngine::BaseModel* model, const EnemySpawn& spawnData, YKEngine::Camera* railCamera, Player* player)
{
	bulletType_ = EnemyBulletType::kHoming;

	BaseEnemy::Initialize(model, spawnData, railCamera, player);
}
