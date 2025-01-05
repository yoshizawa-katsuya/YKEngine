#include "BaseEnemy.h"
#include <numbers>
#include "Rigid3dObject.h"
#include "Camera.h"
#include "Matrix.h"

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

void BaseEnemy::Update()
{
	Collider::Update();

	HPGaugeUpdate();
}

void BaseEnemy::Draw(Camera* camera)
{
	Collider::Draw(camera);

}

void BaseEnemy::SetHPGaugeModel(BaseModel* model)
{
	hpGauge_.object_ = std::make_unique<Rigid3dObject>();
	hpGauge_.object_->Initialize(model);

	hpGauge_.worldTransform_.Initialize();
}

void BaseEnemy::TakeHammer(uint32_t power)
{
	status_.HP_ -= power;
	if (status_.HP_ <= 0) {
		status_.isAlive_ = false;
	}
}

void BaseEnemy::HPGaugeUpdate()
{
	hpGauge_.worldTransform_.translation_.x = GetCenterPosition().x - radius_;
	hpGauge_.worldTransform_.scale_.x = static_cast<float>(status_.HP_) / static_cast<float>(status_.maxHP_);
	
}

void BaseEnemy::HPGaugeDraw(Camera* camera, const Matrix4x4& billbordMatrix)
{
	hpGauge_.worldTransform_.UpdateMatrix(billbordMatrix);

	hpGauge_.object_->WorldTransformUpdate(hpGauge_.worldTransform_);
	hpGauge_.object_->CameraUpdate(camera);
	hpGauge_.object_->Draw();
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

BaseEnemy::StatusWork::StatusWork()
	: maxHP_(5)
	, HP_(maxHP_)
	, isAlive_(true)
{
}