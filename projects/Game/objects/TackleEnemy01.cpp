#include "TackleEnemy01.h"
#include "bullet/BaseBullet.h"
#include "Player.h"
#include "TransformHelpers.h"

using namespace YKEngine;

void TackleEnemy01::Initialize(YKEngine::BaseModel* model, const EnemySpawn& spawnData, YKEngine::Camera* railCamera, Player* player)
{
	BaseEnemy::Initialize(model, spawnData, railCamera, player);
	// ターゲットの初期化
	target_.Initialize();
	WorldTransform* targetTransform = player->GetWorldTransform();
	target_.parent_ = targetTransform->parent_;
	target_.translation_ = targetTransform->translation_;
	targetRadius_ = player->GetRadius();
}

void TackleEnemy01::Update()
{
	target_.UpdateMatrix();

	BaseEnemy::Update();
}

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
		Die({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
		Disappear();
	}
}

void TackleEnemy01::MainInitialize()
{
	//回転
	Rotate();
}

void TackleEnemy01::UpdateApproach()
{
	//レールカメラに映っていたらメインフェーズへ
	if (IsVisible(railCamera_))
	{
		phase_ = Phase::kMain;
		MainInitialize();
	}

}

void TackleEnemy01::UpdateMain()
{
	// 移動
	Move();

	//ダメージリアクション処理
	DamageReaction();

	//レールカメラに映っていなかったら離脱フェーズへ
	if (!hasRail_ && !IsVisible(railCamera_))
	{
		phase_ = Phase::kLeave;
	}
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

}

void TackleEnemy01::Move()
{
	// 移動
	// ホーミング移動
	if (isHoming_)
	{
		//ターゲットに向かって移動する
		Vector3 direction = target_.GetWorldPosition() - worldTransform_.GetWorldPosition();
		//ゴーストと衝突したらホーミング解除
		if (Length(direction) < radius_ + targetRadius_)
		{
			isHoming_ = false;
			return;
		}
		direction = Normalize(direction);
		velocity_ = direction * speed_;
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

void TackleEnemy01::Die(const YKEngine::Vector3& bulletVelocity, const YKEngine::Vector3& bulletPosition)
{
	BaseEnemy::Die(bulletVelocity, bulletPosition);
	// ホーミング解除
	isHoming_ = false;
}

void TackleEnemy01::Rotate()
{
	if (isDead_)
	{
		// 死亡時の回転処理
		characterWorldTransform_.rotation_ += rotateVector_;
		return;
	}
	// プレイヤーの方向を向く
	Vector3 toPosition = target_.GetWorldPosition();
	direction_ = toPosition - GetWorldPosition();
	Vector3 targetRotation = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction_);
	worldTransform_.rotation_ = targetRotation;
}
