#include "Enemy01.h"

Enemy01::Enemy01()
	: speed_(0.05f)
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
	if (lrDirection_ == LRDirection::kRight) {
		worldTransform_.translation_.x += speed_;
		if (worldTransform_.translation_.x > 20.0f) {
			worldTransform_.translation_.x = 20.0f;
			lrDirection_ = LRDirection::kLeft;
			targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}
	else {
		worldTransform_.translation_.x -= speed_;
		if (worldTransform_.translation_.x < -20.0f) {
			worldTransform_.translation_.x = -20.0f;
			lrDirection_ = LRDirection::kRight;
			targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}

	
	BaseEnemy::Update();
}
