#include "BaseEnemy.h"
#include <numbers>
#include "Rigid3dObject.h"

uint32_t BaseEnemy::nextSerialNumber_ = 0;

BaseEnemy::BaseEnemy()
{
	radius_ = 0.5f;
	//シリアル番号を振る
	serialNumber_ = nextSerialNumber_;
	//次の番号を1加算
	nextSerialNumber_++;
}

BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Initialize(BaseModel* model, const Vector3& translate)
{
	Collider::Initialize(model);

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.translation_ = translate;
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