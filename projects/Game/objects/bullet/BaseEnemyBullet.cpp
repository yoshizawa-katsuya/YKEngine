#include "BaseEnemyBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"
#include "Player.h"
#include "Lerp.h"

using namespace YKEngine;

void BaseEnemyBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle, Player* target, float speed)
{
	target_ = target;
	speed_ = speed;
	BaseBullet::Initialize(model, position, velocity, textureHandle);
	Collider::SetTypeID(CollisionTypeIdDef::kEnemyBullet);

}

void BaseEnemyBullet::Update(Camera* railCamera)
{
	BaseBullet::Update();

	//画面外に出たら消す
	if (!IsVisible(railCamera))
	{
		isDead_ = true;
	}
}

void BaseEnemyBullet::OnCollision(Collider* other)
{
	CollisionTypeIdDef typeID = other->GetTypeID();

	if (typeID == CollisionTypeIdDef::kPlayer)
	{
		isDead_ = true;
	}
}

void BaseEnemyBullet::Move()
{
	if (isHoming_)
	{
		Homig();
	}
	
	//座標を移動させる
	worldTransform_.translation_ += velocity_;
}

void BaseEnemyBullet::Homig()
{
	//ターゲットに向かって移動する
	Vector3 direction = target_->GetWorldPosition() - worldTransform_.GetWorldPosition();
	//ある程度近づいたらホーミングをやめる
	if (Length(direction) < 5.0f)
	{
		isHoming_ = false;
		return;
	}
	direction = Normalize(direction);
	Vector3 homing = direction * speed_;
	//補間を使ってなめらかにする
	velocity_ = Lerp(velocity_, homing, 0.13f);
}


