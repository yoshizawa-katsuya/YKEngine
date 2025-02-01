#include "Enemy.h"
#include "Matrix.h"
#include "Vector.h"
#include <algorithm>
#include "CollisionTypeIdDef.h"
#include <numbers>

uint32_t Enemy::nextSerialNumber_ = 0;

Enemy::Enemy() {

	//シリアル番号を振る
	serialNumber_ = nextSerialNumber_;
	//次の番号を1加算
	++nextSerialNumber_;

}

void Enemy::Initialize(const std::vector<RigidModel*>& models, RigidModel* colliderModel) {

	BaseCharacter::Initialize(models, colliderModel);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));

	InitializeRollArmGimmick();

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;
	
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = Vector3(-1.7f, 1.3f, 0.0f);
	worldTransformL_arm_.parent_ = &worldTransformBody_;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = Vector3(1.7f, 1.3f, 0.0f);
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	
}

void Enemy::InitializeRollArmGimmick() {

	rollArmParameter_ = 0.0f;

}

void Enemy::InitializeHitEffect(RigidModel* model) {

	objectHitEffect_ = std::make_unique<Rigid3dObject>();
	objectHitEffect_->Initialize(model);

	worldTransformHitEffect_.Initialize();
	worldTransformHitEffect_.translation_ = Vector3(0.0f, 1.3f, 0.0f);
	worldTransformHitEffect_.parent_ = &worldTransform_;
	hitEffectParameter_ = 0.0f;

}

void Enemy::Update() {

	worldTransform_.rotation_.y += 0.02f;

	//移動速度
	const float kSpeed = 0.3f;
	Vector3 velocity(0, 0, kSpeed);

	//移動方向を向きに合わせる
	velocity = TransformNormal(velocity, worldTransform_.worldMatrix_);

	//移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);

	//パーツギミック
	UpdateRollArmGimmick();

	//ヒットエフェクト
	UpdateHitEffect();

	BaseCharacter::Update();

	worldTransformBody_.UpdateMatrix();
	objects_[kModelIndexBody]->WorldTransformUpdate(worldTransformBody_);

	worldTransformL_arm_.UpdateMatrix();
	objects_[kModelIndexL_arm]->WorldTransformUpdate(worldTransformL_arm_);

	worldTransformR_arm_.UpdateMatrix();
	objects_[kModelIndexR_arm]->WorldTransformUpdate(worldTransformR_arm_);

}

void Enemy::UpdateRollArmGimmick() {

	//   腕振りのサイクル<frame>
	const uint16_t cycle = 60;

	// 1フレームでのパラメーター加算値
	const float step = 2.0f * static_cast<float>(std::numbers::pi) / cycle;

	// パラメーターを1ステップ分加算
	rollArmParameter_ += step;
	// 2πを超えたら0に戻す
	rollArmParameter_ = std::fmod(rollArmParameter_, 2.0f * static_cast<float>(std::numbers::pi));

	// 腕振りの振幅<m>
	const float amplitude = 4.0f;

	float rollArm = std::clamp(std::sin(rollArmParameter_) * amplitude, -2.0f, 2.0f);


	// 腕振りを座標に反映
	worldTransformL_arm_.translation_.z = rollArm;
	worldTransformR_arm_.translation_.z = rollArm;

	

}

void Enemy::UpdateHitEffect() {

	if (hitEffectParameter_ <= 0.0f) {
		return;
	}

	float scale;
	scale = 5.0f - (hitEffectParameter_ * 2.0f);
	worldTransformHitEffect_.scale_ = Vector3(scale, scale, scale);
	hitEffectParameter_ -= 1.0f / 60.0f;
	worldTransformHitEffect_.UpdateMatrix();
	objectHitEffect_->WorldTransformUpdate(worldTransformHitEffect_);
}

void Enemy::Draw(Camera* camera) {

	objects_[kModelIndexBody]->CameraUpdate(camera);
	objects_[kModelIndexBody]->Draw();

	objects_[kModelIndexL_arm]->CameraUpdate(camera);
	objects_[kModelIndexL_arm]->Draw();

	objects_[kModelIndexR_arm]->CameraUpdate(camera);
	objects_[kModelIndexR_arm]->Draw();
	
	DrawHitEffect(camera);
	
}

void Enemy::DrawHitEffect(Camera* camera) {

	if (hitEffectParameter_ <= 0.0f) {
		return;
	}

	objectHitEffect_->CameraUpdate(camera);
	objectHitEffect_->Draw();

}

void Enemy::Hit() {

	hitEffectParameter_ = 1.0f;

}

Vector3 Enemy::GetCenterPosition() const{ 

	//見た目上の中心点オフセット
	const Vector3 offset = {0.0f, 2.0f, 0.0f};
	//ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.worldMatrix_);
	return worldPos;

}
