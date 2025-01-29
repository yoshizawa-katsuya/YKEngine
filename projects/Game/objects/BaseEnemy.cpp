#include "BaseEnemy.h"
#include <numbers>
#include "Rigid3dObject.h"
#include "Camera.h"
#include "Matrix.h"
#include "Lerp.h"
#include "Easing.h"
#include "Audio.h"
#include "ThreadPool.h"

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

	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
	HPGaugeUpdate();

}

void BaseEnemy::Update()
{
	ThreadPool::GetInstance()->enqueueTask([&]() {

		if (lrDirection_ == LRDirection::kRight) {
			worldTransform_.translation_.x += speed_;
			if (worldTransform_.translation_.x > 20.5f - radius_) {
				worldTransform_.translation_.x = 20.5f - radius_;
				lrDirection_ = LRDirection::kLeft;
				targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = 0.0f;
			}
		}
		else {
			worldTransform_.translation_.x -= speed_;
			if (worldTransform_.translation_.x < -20.5f + radius_) {
				worldTransform_.translation_.x = -20.5f + radius_;
				lrDirection_ = LRDirection::kRight;
				targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = 0.0f;
			}
		}

		// 旋回制御
		if (turnTimer_ < 1.0f) {
			turnTimer_ += 1.0f / 12.0f;

			if (turnTimer_ > 1.0f) {
				turnTimer_ = 1.0f;
			}
			// 自キャラの角度を設定する
			//worldTransform_.rotation_.y = turnFirstRotationY_ * EaseOut(turnTimer_) + destinationRotationY * (1 - EaseOut(turnTimer_));
			worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, targetAngle_, EaseOut(turnTimer_));

		}

		Collider::Update();

		HPGaugeUpdate();
	});
}

void BaseEnemy::OnCollision()
{
	status_.bombNum_++;
}

void BaseEnemy::Draw(Camera* camera)
{
	Collider::Draw(camera);

}

void BaseEnemy::SetHPGaugeModel(BaseModel* model, BaseModel* frameModel)
{
	hpGauge_.object_ = std::make_unique<Rigid3dObject>();
	hpGauge_.object_->Initialize(model);

	hpGauge_.worldTransform_.Initialize();
	hpGauge_.worldTransform_.translation_.y = worldTransform_.translation_.y;

	hpGauge_.darkObject_ = std::make_unique<Rigid3dObject>();
	hpGauge_.darkObject_->Initialize(model);

	hpGauge_.darkWorldTransform_.Initialize();
	hpGauge_.darkWorldTransform_.translation_.y = worldTransform_.translation_.y;
	hpGauge_.darkWorldTransform_.translation_.z = 0.001f;

	hpGauge_.frameObject_ = std::make_unique<Rigid3dObject>();
	hpGauge_.frameObject_->Initialize(frameModel);

	hpGauge_.frameWorldTransform_.Initialize();
	hpGauge_.frameWorldTransform_.translation_.y = worldTransform_.translation_.y;
}

void BaseEnemy::TakeHammer(uint32_t power)
{
	status_.HP_ -= power + status_.bombNum_;
	status_.bombNum_ = 0;
	if (status_.HP_ <= 0) {
		status_.isAlive_ = false;
	}
}

bool BaseEnemy::isHavingBomb()
{
	return (status_.bombNum_ > 0);
}

void BaseEnemy::HPGaugeUpdate()
{
	hpGauge_.worldTransform_.translation_.x = GetCenterPosition().x - radius_;
	hpGauge_.worldTransform_.scale_.x = (static_cast<float>(status_.HP_) - static_cast<float>(status_.bombNum_)) / static_cast<float>(status_.maxHP_);
	
	hpGauge_.darkWorldTransform_.translation_.x = hpGauge_.worldTransform_.translation_.x;
	hpGauge_.darkWorldTransform_.scale_.x = static_cast<float>(status_.HP_) / static_cast<float>(status_.maxHP_);

	hpGauge_.frameWorldTransform_.translation_.x = hpGauge_.worldTransform_.translation_.x;
}

void BaseEnemy::HPGaugeDraw(Camera* camera, const Matrix4x4& billbordMatrix)
{
	hpGauge_.worldTransform_.UpdateMatrix(billbordMatrix);

	hpGauge_.object_->WorldTransformUpdate(hpGauge_.worldTransform_);
	hpGauge_.object_->CameraUpdate(camera);
	hpGauge_.object_->Draw();

	hpGauge_.darkWorldTransform_.UpdateMatrix(billbordMatrix);

	hpGauge_.darkObject_->WorldTransformUpdate(hpGauge_.darkWorldTransform_);
	hpGauge_.darkObject_->CameraUpdate(camera);
	hpGauge_.darkObject_->Draw(textureHandleDarkRed_);

	hpGauge_.frameWorldTransform_.UpdateMatrix(billbordMatrix);

	hpGauge_.frameObject_->WorldTransformUpdate(hpGauge_.frameWorldTransform_);
	hpGauge_.frameObject_->CameraUpdate(camera);
	hpGauge_.frameObject_->Draw();
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
	: maxHP_(7)
	, HP_(maxHP_)
	, bombNum_(0)
	, isAlive_(true)
{
}