#include "TackleEnemy02.h"
#include "bullet/BaseBullet.h"

using namespace YKEngine;

TackleEnemy02::~TackleEnemy02()
{
}

void TackleEnemy02::OnCollision(Collider* other)
{
	if (other->GetTypeID() == CollisionTypeIdDef::kPlayerBullet)
	{
		OnCollisionPlayerBullet(other);
	}
	else if (other->GetTypeID() == CollisionTypeIdDef::kPlayer)
	{
		// プレイヤーと衝突したら自滅
		isDead_ = true;
		hasRail_ = false;
		Disappear();
	}
}

void TackleEnemy02::UpdateApproach()
{

	//回転
	Rotate();

}

void TackleEnemy02::UpdateLeave()
{
	//離脱タイマーをカウント
	leaveTimer_ += 1.0f / 60.0f;
	float leaveTime = 1.0f; // 離脱までの時間（秒）
	if (leaveTimer_ > leaveTime)
	{
		Disappear();
	}

	//回転
	Rotate();
}

void TackleEnemy02::Move()
{
	// 移動
	// ホーミング移動
	if (isHoming_)
	{
		Vector3 normalDirection = Normalize(direction_);
		velocity_ = normalDirection * speed_;
		worldTransform_.translation_ += velocity_;
	}
	else
	{
		BaseEnemy::Move();
	}
}

void TackleEnemy02::SetColliderID()
{
	Collider::SetTypeID(CollisionTypeIdDef::kTackleEnemy);
}

void TackleEnemy02::DeadInitialize()
{
	// ホーミング解除
	isHoming_ = false;

	BaseEnemy::DeadInitialize();
}
