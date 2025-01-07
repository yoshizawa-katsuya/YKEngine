#include "Enemy01.h"
#include "Rigid3dObject.h"

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

void Enemy01::SetHPGaugeModel(BaseModel* model)
{
	hpGauge_.object_ = std::make_unique<Rigid3dObject>();
	hpGauge_.object_->Initialize(model);

	hpGauge_.worldTransform_.Initialize();
	hpGauge_.worldTransform_.translation_.y = worldTransform_.translation_.y - 0.2f;

	hpGauge_.frameObject_ = std::make_unique<Rigid3dObject>();
	hpGauge_.frameObject_->Initialize(model);

	hpGauge_.frameWorldTransform_.Initialize();
	hpGauge_.frameWorldTransform_.translation_.y = worldTransform_.translation_.y - 0.2f;
	hpGauge_.frameWorldTransform_.translation_.z = 0.001f;
}
