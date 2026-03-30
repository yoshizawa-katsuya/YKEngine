#include "BaseEnemy.h"
#include "cassert"
#include "Vector3.h"
#include "Matrix.h"
#include "Player.h"
#include "TransformHelpers.h"
#include "Lerp.h"
#include "Curve.h"
#include "manager/EffectManager.h"
#include "Camera.h"
#include "Random.h"
#include "bullet/BaseBullet.h"
#include <algorithm>
#include "EnemyApproachState.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

BaseEnemy::~BaseEnemy() 
{
}

void BaseEnemy::Initialize(BaseModel* model, const EnemySpawn& spawnData, Camera* railCamera, Player* player)
{

	BaseCharacter::Initialize(model);
	SetColliderID();
	player_ = player;
	railCamera_ = railCamera;

	globalVariables_ = GlobalVariables::GetInstance();

	//速さをレベルデータから取得
	if (spawnData.speed.has_value())
	{
		speed_ = spawnData.speed.value();
	}

	if (!spawnData.controlPoints.empty())
	{
		hasRail_ = true;
		CreateSplineCurve(spawnData.controlPoints);
		worldTransform_.translation_ = controlPoints_[0];
	}
	else
	{
		worldTransform_.translation_ = spawnData.position;
		//移動方向を初期化
		Matrix4x4 rotateMatrix = MakeRotateMatrix(spawnData.rotation);
		velocity_ *= speed_;
		velocity_ = TransformNormal(velocity_, rotateMatrix);
	}

	viewPortMatrix_ = &DirectXCommon::GetInstance()->GetViewPortMatrix();

	waitTime_ = spawnData.waitTime;
	worldTransform_.rotation_ = spawnData.rotation;

	//ステートマシンの初期化と開始、最初のステートは接近ステートにする
	stateMachine_ = std::make_unique<StateMachine<EnemyStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<EnemyApproachState>();

}

void BaseEnemy::Update() {

	stateMachine_->Update();

	BaseCharacter::Update();
}

void BaseEnemy::OnCollision(Collider* other)
{
	if (other->GetTypeID() == CollisionTypeIdDef::kPlayerBullet) 
	{
		
		OnCollisionPlayerBullet(other);
		
	}
}

Vector3 BaseEnemy::GetWorldPosition() 
{

	return worldTransform_.GetWorldPosition();

}

Vector2 BaseEnemy::GetScreenPosition(Camera* camera) {
	
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(camera->GetViewProjection(), *viewPortMatrix_);

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	Vector3 screenPosition = Transform(GetWorldPosition(), matViewProjectionViewport);

	return { screenPosition.x, screenPosition.y };
}

void BaseEnemy::SetColliderID()
{
	Collider::SetTypeID(CollisionTypeIdDef::kEnemy);
}

void BaseEnemy::MainInitialize() 
{
}

void BaseEnemy::LeaveInitialize()
{
	leaveTimer_ = 0.0f;
}

void BaseEnemy::DeadInitialize()
{
	isDead_ = true;
	hasRail_ = false;

	// 死亡時の速度を設定
	Vector3 directionBullet = Normalize(dieInfo_->bulletVelocity);
	Vector3 directionToEnemy = Normalize(GetWorldPosition() - dieInfo_->bulletPosition);

	const std::string& groupName = JsonKey::Enemy::kGroupName;

	blowAwaySpeed_ = globalVariables_->GetFloatValue(groupName, JsonKey::Enemy::kBrowAwaySpeed);
	const float kDirectionWeightBullet = globalVariables_->GetFloatValue(groupName, JsonKey::Enemy::kDirectionWeightBullet);
	const float kDirectionWeightToEnemy = 1.0f - kDirectionWeightBullet;
	velocity_ = Normalize(directionBullet * kDirectionWeightBullet + directionToEnemy * kDirectionWeightToEnemy) * blowAwaySpeed_;

	// ランダムな回転ベクトルを設定
	const Vector3 kRotateVectorMin = { -1.0f, -1.0f, -1.0f };
	const Vector3 kRotateVectorMax = { 1.0f, 1.0f, 1.0f };

	const float kRotateSpeedMin = globalVariables_->GetFloatValue(groupName, JsonKey::Enemy::kDeadRotateSpeedMin);
	const float kRotateSpeedMax = globalVariables_->GetFloatValue(groupName, JsonKey::Enemy::kDeadRotateSpeedMax);

	Random* random = Random::GetInstance();
	rotateVector_ = Normalize(random->GetVector3(kRotateVectorMin, kRotateVectorMax)) * random->GetFloat(kRotateSpeedMin, kRotateSpeedMax);
}

void BaseEnemy::UpdateApproach()
{

	// 移動
	Move();

	//回転
	Rotate();

}

void BaseEnemy::UpdateMain() 
{
	// 移動
	Move();

	//回転
	Rotate();

	//ダメージリアクション処理
	DamageReaction();

}

void BaseEnemy::UpdateLeave()
{
	//離脱タイマーをカウント
	leaveTimer_ += 1.0f / 60.0f;
	const float leaveTime = globalVariables_->GetFloatValue(JsonKey::Enemy::kGroupName, JsonKey::Enemy::kLeaveTime);
	if (leaveTimer_ > leaveTime) 
	{
		Disappear();
	}

	// 移動
	Move();

	//回転
	Rotate();
}

void BaseEnemy::UpdateDead()
{
	const float kDeadTime = globalVariables_->GetFloatValue(JsonKey::Enemy::kGroupName, JsonKey::Enemy::kDeadTime);	// 死亡してから完全に消滅するまでの時間

	// 1フレームごとにデッドタイマーをカウントアップ
	deadTimer_ += 1.0f / 60.0f;

	if (deadTimer_ > kDeadTime)
	{
		// 完全に消滅
		Disappear();
	}

	// 移動
	velocity_ = Normalize(velocity_) * Lerp(blowAwaySpeed_, 0.0f, deadTimer_ / kDeadTime);
	// 画面内にいる間は移動を続ける
	if (IsVisible(railCamera_))
	{
		Move();
	}
	
	// 回転
	Rotate();

	// エフェクト生成
	EffectManager::GetInstance()->SpawnEffect(EffectType::kEnemyBrowAway01, GetWorldPosition(), 10);
}

bool BaseEnemy::IsLeave()
{
	return !IsVisible(railCamera_) && !hasRail_;
}

bool BaseEnemy::IsMain()
{
	return IsVisible(railCamera_);
}

bool BaseEnemy::IsInRailCamera()
{
	return IsVisible(railCamera_);
}

void BaseEnemy::CreateSplineCurve(const std::vector<Vector3>& controlPoints)
{
	// レベルデータから制御点を取得
	for (Vector3 controlPoint : controlPoints) {
		controlPoints_.push_back(controlPoint);
	}
	// Catmull-Romスプラインのポイントを生成
	corvePoints_ = GenerateCatmullRomSplinePoints(controlPoints_, kSegmentCount_);
}

void BaseEnemy::Move()
{
	if (hasRail_)
	{
		MoveAlongRail();
	}
	else
	{
		worldTransform_.translation_ += velocity_;
		// 地面より下に行かないようにする
		worldTransform_.translation_.y = (std::max)(worldTransform_.translation_.y, 0.0f);
	}
}

void BaseEnemy::Rotate()
{
	if (isDead_) 
	{
		// 死亡時の回転処理
		characterWorldTransform_.rotation_ += rotateVector_;
		return;
	}
	// プレイヤーの方向を向く
	Vector3 toPosition = player_->GetWorldPosition();
	direction_ = toPosition - GetWorldPosition();
	Vector3 targetRotation = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction_);
	worldTransform_.rotation_ = LerpAngle(worldTransform_.rotation_, targetRotation, 0.1f);
}

void BaseEnemy::MoveAlongRail()
{
	if (corvePoints_.size() > moveCount_) 
	{
		//残りの移動距離計算用の変数
		float remainingMoveDistance = speed_;

		Vector3 moveDirection{};

		// 位置を更新する処理
		while (remainingMoveDistance > 0.0f && corvePoints_.size() > moveCount_)
		{
			moveDirection = corvePoints_[moveCount_] - worldTransform_.translation_;
			float distance = Length(moveDirection);

			// ほぼゼロ距離の場合はスキップ
			if (distance < 0.001f) {
				moveCount_++;
				continue;
			}

			if (distance < remainingMoveDistance)
			{
				// 次のポイントに移動する距離が残りの距離よりも短い場合、次のポイントに移動
				worldTransform_.translation_ = corvePoints_[moveCount_];
				remainingMoveDistance -= distance;
				moveCount_++;

			}
			else
			{
				// 残りの距離が次のポイントまでの距離よりも長い場合、次のポイントに向かって移動
				Vector3 normalizeDirection = Normalize(moveDirection);
				// 残りの距離を考慮してカメラの位置を更新
				worldTransform_.translation_ += normalizeDirection * remainingMoveDistance;
				remainingMoveDistance = 0.0f;
			}
		}

		// レールの終点に到達した場合
		if (corvePoints_.size() <= moveCount_)
		{
			velocity_ = speed_ * Normalize(moveDirection);
			hasRail_ = false;
		}
	}
}

void BaseEnemy::DamageReactionInitialize()
{
	// ダメージリアクションの時間を取得
	damageReactionTimer_ = globalVariables_->GetFloatValue(JsonKey::Enemy::kGroupName, JsonKey::Enemy::kDamageReactionTime);
}

void BaseEnemy::DamageReaction()
{
	if (damageReactionTimer_ < 0.0f)
	{
		// ダメージリアクション終了
		characterWorldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
		return;
	}

	damageReactionTimer_ -= 1.0f / 60.0f;

	//乱数での移動量の設定
	const float kMoveRange = globalVariables_->GetFloatValue(JsonKey::Enemy::kGroupName, JsonKey::Enemy::kDamageReactionMoveRange);

	characterWorldTransform_.translation_ = Random::GetInstance()->GetVector3(-kMoveRange, kMoveRange);
}

void BaseEnemy::OnCollisionPlayerBullet(Collider* other)
{

	// 弾と衝突したら体力を減らす
	BaseBullet* bullet = dynamic_cast<BaseBullet*>(other);
	assert(bullet);

	hitPoint_ -= bullet->GetAttackPower();

	// ダメージリアクション開始
	DamageReactionInitialize();

	// エフェクト生成
	EffectManager::GetInstance()->SpawnEffect(EffectType::kHit01, worldTransform_.GetWorldPosition());
	EffectManager::GetInstance()->SpawnEffect(EffectType::kHit02, worldTransform_.GetWorldPosition(), 10);

	if (hitPoint_ > 0) 
	{
		return;
	}
	// 体力が0以下になったら死亡
 	dieInfo_ = { bullet->GetVelocity(), bullet->GetCenterPosition() };

}

void BaseEnemy::Disappear()
{
	isDisappear_ = true;

	// 死亡している場合のみエフェクトを生成
	if (isDead_)
	{
		// エフェクト生成
		EffectManager::GetInstance()->SpawnEffect(EffectType::kEnemyEnd01, worldTransform_.GetWorldPosition(), 50);
	}
	
}
