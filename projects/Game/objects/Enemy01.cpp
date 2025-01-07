#include "Enemy01.h"

Enemy01::Enemy01()
{
}

Enemy01::~Enemy01()
{
}

void Enemy01::Initialize(BaseModel* model, const EulerTransform& transform)
{
	BaseEnemy::Initialize(model, transform);

}

void Enemy01::Update()
{
	
	BaseEnemy::Update();
}
