#include "TackleEnemy01.h"
#include "bullet/BaseBullet.h"

using namespace YKEngine;

TackleEnemy01::~TackleEnemy01()
{
}

void TackleEnemy01::OnCollision(Collider* other)
{
	if (other->GetTypeID() == CollisionTypeIdDef::kPlayerBullet)
	{
		OnCollisionPlayerBullet(other);
	}
	else if (other->GetTypeID() == CollisionTypeIdDef::kPlayer)
	{
		// プレイヤーと衝突したら自滅
		Die({0.0f, 0.0f, 0.0f});
		Disappear();
	}
}

void TackleEnemy01::UpdateApproach()
{
	//レールカメラに映っていたらメインフェーズへ
	if (IsVisible(railCamera_))
	{
		phase_ = Phase::kMain;
		MainInitialize();
	}

	//回転
	Rotate();

	//ダメージリアクション
	DamageReaction();
}

void TackleEnemy01::UpdateMain()
{
	
	//回転
	Rotate();

	// 移動
	Move();

	//レールカメラに映っていなかったら離脱フェーズへ
	if (!hasRail_ && !IsVisible(railCamera_))
	{
		phase_ = Phase::kLeave;
	}

	//ダメージリアクション
	DamageReaction();
}

void TackleEnemy01::UpdateLeave()
{
	//離脱タイマーをカウント
	leaveTimer_ += 1.0f / 60.0f;
	float leaveTime = 1.0f; // 離脱までの時間（秒）
	if (leaveTimer_ > leaveTime)
	{
		Disappear();
	}

	//画面内に戻ってきたらメインフェーズへ
	if (IsVisible(railCamera_))
	{
		phase_ = Phase::kMain;
		leaveTimer_ = 0.0f;
		MainInitialize();
	}

	//回転
	Rotate();
}

void TackleEnemy01::Move()
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

void TackleEnemy01::SetColliderID()
{
	Collider::SetTypeID(CollisionTypeIdDef::kTackleEnemy);
}

void TackleEnemy01::Die(const YKEngine::Vector3& bulletVelocity)
{
	BaseEnemy::Die(bulletVelocity);
	// ホーミング解除
	isHoming_ = false;
}
