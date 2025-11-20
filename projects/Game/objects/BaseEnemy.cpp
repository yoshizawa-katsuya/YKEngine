#include "BaseEnemy.h"
#include "cassert"
#include "Vector3.h"
#include "Matrix.h"
#include "Player.h"
#include "GameScene.h"
#include "WinApp.h"
#include "TransformHelpers.h"
#include "Lerp.h"
#include "Curve.h"

BaseEnemy::~BaseEnemy() 
{
}

void BaseEnemy::Initialize(BaseModel* model, const EnemySpawn& spawnData, Matrix4x4* viewPortMatrix, Camera* railCamera) 
{

	BaseCharacter::Initialize(model);
	SetColliderID();

	railCamera_ = railCamera;

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

	viewPortMatrix_ = viewPortMatrix;

	worldTransform_.rotation_ = spawnData.rotation;
}

void BaseEnemy::Update() {

	switch (phase_) {
	case Phase::kApproach:
		UpdateApproach();
		break;
	case Phase::kMain:
		UpdateMain();
		break;
	case Phase::kLeave:
		UpdateLeave();
		break;
	}

	BaseCharacter::Update();
}

void BaseEnemy::OnCollision(Collider* other)
{
	if (other->GetTypeID() == CollisionTypeIdDef::kPlayerBullet) 
	{
		// 弾と衝突したら体力を減らす
		BaseBullet* bullet = dynamic_cast<BaseBullet*>(other);
		assert(bullet);

		hitPoint_ -= bullet->GetAttackPower();

		if (hitPoint_ > 0) {
			return;
		}
		// 体力が0以下になったら死亡
		isDead_ = true;
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
	//発射タイマーを初期化
	fireTimer = kFireInterval;

}

void BaseEnemy::UpdateApproach()
{

	// 移動
	Move();

	//レールカメラに映っていたらメインフェーズへ
	if (IsVisible(railCamera_))
	{
		phase_ = Phase::kMain;
		MainInitialize();
	}

	//回転
	Rotate();

}

void BaseEnemy::UpdateMain() 
{

	//発射タイマーカウントダウン
	fireTimer--;
	//指定時間に達した
	if (fireTimer == 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}

	// 移動
	Move();

	//回転
	Rotate();

	//レールカメラに映っていなかったら離脱フェーズへ
	if (!hasRail_ && !IsVisible(railCamera_))
	{
		phase_ = Phase::kLeave;
	}

}

void BaseEnemy::UpdateLeave()
{
	//離脱タイマーをカウント
	leaveTimer_ += 1.0f / 60.0f;
	float leaveTime = 1.0f; // 離脱までの時間（秒）
	if (leaveTimer_ > leaveTime) 
	{
		isDead_ = true;
	}

	//画面内に戻ってきたらメインフェーズへ。画面の揺れなどで戻ってきた場合を考慮し、離脱タイマーもリセットする
	if (IsVisible(railCamera_))
	{
		phase_ = Phase::kMain;
		leaveTimer_ = 0.0f;
		MainInitialize();
	}

	// 移動
	Move();

	//回転
	Rotate();
}

void BaseEnemy::Fire() {

	//弾の速さ
	const float kBulletSpeed = 0.5f;

	Vector3 velocity = Normalize(direction_);
	velocity = Multiply(kBulletSpeed, velocity);

	// 弾を生成し、初期化
	enemyBulletManager_->AddEnemyBullet(GetWorldPosition(), velocity, player_, kBulletSpeed);

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
	}
}

void BaseEnemy::Rotate()
{
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
