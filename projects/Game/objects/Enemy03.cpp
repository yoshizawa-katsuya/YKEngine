#include "Enemy03.h"
#include "Rigid3dObject.h"

Enemy03::Enemy03()
{
}

Enemy03::~Enemy03()
{
}

void Enemy03::Initialize(BaseModel* model, const EulerTransform& transform)
{
	BaseEnemy::Initialize(model, transform);

	radius_ = 1.0f;
	speed_ = 0.01f;
	status_.maxHP_ = 20;
	status_.HP_ = status_.maxHP_;
}

void Enemy03::SetHPGaugeModel(BaseModel* model)
{
	hpGauge_.object_ = std::make_unique<Rigid3dObject>();
	hpGauge_.object_->Initialize(model);

	hpGauge_.worldTransform_.Initialize();
	hpGauge_.worldTransform_.translation_.y = worldTransform_.translation_.y + 0.2f;

	hpGauge_.darkFrameObject_ = std::make_unique<Rigid3dObject>();
	hpGauge_.darkFrameObject_->Initialize(model);

	hpGauge_.darkWorldTransform_.Initialize();
	hpGauge_.darkWorldTransform_.translation_.y = worldTransform_.translation_.y + 0.2f;
	hpGauge_.darkWorldTransform_.translation_.z = 0.001f;
}

void Enemy03::HPGaugeUpdate()
{
	hpGauge_.worldTransform_.translation_.x = GetCenterPosition().x - radius_;
	hpGauge_.worldTransform_.scale_.x = (static_cast<float>(status_.HP_) - static_cast<float>(status_.bombNum_)) / static_cast<float>(status_.maxHP_) * 2.0f;

	hpGauge_.darkWorldTransform_.translation_.x = hpGauge_.worldTransform_.translation_.x;
	hpGauge_.darkWorldTransform_.scale_.x = static_cast<float>(status_.HP_) / static_cast<float>(status_.maxHP_) * 2.0f;
}
