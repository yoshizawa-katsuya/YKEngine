#include "TackleEnemy01.h"
#include "Player.h"
#include "TransformHelpers.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void TackleEnemy01::Initialize(YKEngine::BaseModel* model, const EnemySpawn& spawnData, YKEngine::Camera* railCamera, Player* player)
{
	BaseEnemy::Initialize(model, spawnData, railCamera, player);

	if (!spawnData.speed.has_value())
	{
		speed_ = globalVariables_->GetFloatValue(JsonKey::Enemy::Tackle01::kGroupName, JsonKey::Enemy::kDefaultSpeed);
	}

	hitPoint_ = 3; // ヒットポイントを設定。TODO:jsonから取得するようにする

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
		isDead_ = true;
		hasRail_ = false;
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
}

void TackleEnemy01::UpdateLeave()
{
	// 移動
	Move();

	//ダメージリアクション処理
	DamageReaction();

	//離脱タイマーをカウント
	leaveTimer_ += 1.0f / 60.0f;
	float leaveTime = 1.0f; // 離脱までの時間（秒）
	if (leaveTimer_ > leaveTime)
	{
		Disappear();
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
			isLeave_ = true;
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

bool TackleEnemy01::IsLeave()
{
	return BaseEnemy::IsLeave() || isLeave_;
}

bool TackleEnemy01::IsMain()
{
	return false;
}

void TackleEnemy01::DeadInitialize()
{
	// ホーミング解除
	isHoming_ = false;

	BaseEnemy::DeadInitialize();
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
