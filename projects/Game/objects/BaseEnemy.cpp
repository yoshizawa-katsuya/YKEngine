#include "BaseEnemy.h"
#include <numbers>
#include "Rigid3dObject.h"

uint32_t BaseEnemy::nextSerialNumber_ = 0;

BaseEnemy::BaseEnemy()
{
	
}

BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Initialize(BaseModel* model, const EulerTransform& transform)
{
	Collider::Initialize(model);

	radius_ = 0.5f;
	//シリアル番号を振る
	serialNumber_ = nextSerialNumber_;
	//次の番号を1加算
	nextSerialNumber_++;

	if (transform.rotation.y > 0.0f) {
		worldTransform_.rotation_.y = destinationRotationYTable[0];
		lrDirection_ = LRDirection::kRight;
	}
	else {
		worldTransform_.rotation_.y = destinationRotationYTable[1];
		lrDirection_ = LRDirection::kLeft;
	}
	worldTransform_.translation_ = transform.translation;
}
/*
void BaseEnemy::Update()
{
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void BaseEnemy::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}
*/