#include "BasePlayerBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"
#include "manager/EffectManager.h"
#include "BaseEnemy.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void BasePlayerBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& direction, BaseEnemy* targetEnemy)
{
	targetEnemy_ = targetEnemy;

	// 速度計算
	Vector3 velocity = direction * speed_;

	BaseBullet::Initialize(model, position, velocity);
	Collider::SetTypeID(CollisionTypeIdDef::kPlayerBullet);

	worldTransform_.UpdateMatrix();

}

void BasePlayerBullet::Update()
{
	// 回転
	BaseBullet::Rotate();
	const float kRotationSpeed = GlobalVariables::GetInstance()->GetFloatValue(JsonKey::Bullet::Player::kGroupName, JsonKey::Bullet::kRotateSpeed);
	characterWorldTransform_.rotation_.z += kRotationSpeed;

	// 目標追尾
	if (targetEnemy_)
	{
		Vector3 direction = Normalize(targetEnemy_->GetWorldPosition() - worldTransform_.GetWorldPosition());
		velocity_ = direction * speed_;
		// 目標が死んでいたらターゲット解除
		if (targetEnemy_->IsDead())
		{
			targetEnemy_ = nullptr;
		}
	}

	BaseBullet::Update();

	Vector3 particleVelocity = velocity_ * 0.1f; // パーティクルの速度は弾の速度の半分に設定
	EffectManager::GetInstance()->SpawnEffect(EffectType::kPlayerBulletTrac01, worldTransform_.GetWorldPosition(), particleVelocity);
}

void BasePlayerBullet::OnCollision(Collider* other)
{
	CollisionTypeIdDef typeID = other->GetTypeID();

	if (typeID == CollisionTypeIdDef::kEnemy || typeID == CollisionTypeIdDef::kTackleEnemy)
	{
		isDead_ = true;
	}
}
