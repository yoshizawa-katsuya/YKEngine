#include "RailMover.h"
#include "Curve.h"
#include "TransformHelpers.h"
#include "Matrix.h"
#include "ModelPlatform.h"
#include "WaveEvent.h"
#include "manager/EnemySpawnManager.h"
#include "SpeedEvent.h"

void RailMover::Initialize(const std::vector<Vector3>& controlPoints, EnemySpawnManager* enemySpawnManager)
{
	Collider::Initialize();
	typeID_ = CollisionTypeIdDef::kRailMover;

	enemySpawnManager_ = enemySpawnManager;

	CreateSplineCurve(controlPoints);
	// 初期位置をスプラインの最初のポイントに設定
	eye_ = pointsDrawing_[0]; // 初期位置をスプラインの最初のポイントに設定


	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = eye_;

	//向きを更新する処理
	UpdateRotate();

	UpdateOffset();

	worldTransform_.UpdateMatrix();
}

void RailMover::Update()
{
	if (pointsDrawing_.size() > moveCount_) {
		//残りの移動距離計算用の変数
		float remainingMoveDistance = speed_;

		// カメラの位置を更新する処理
		while (remainingMoveDistance > 0.0f && pointsDrawing_.size() > moveCount_)
		{
			Vector3 moveDirection = pointsDrawing_[moveCount_] - eye_;
			float distance = Length(moveDirection);

			// ほぼゼロ距離の場合はスキップ
			if (distance < 0.001f) {
				moveCount_++;
				continue;
			}

			if (distance < remainingMoveDistance)
			{
				// 次のポイントに移動する距離が残りの距離よりも短い場合、次のポイントに移動
				eye_ = pointsDrawing_[moveCount_];
				remainingMoveDistance -= distance;
				moveCount_++;

			}
			else
			{
				// 残りの距離が次のポイントまでの距離よりも長い場合、次のポイントに向かって移動
				Vector3 normalizeDirection = Normalize(moveDirection);
				// 残りの距離を考慮してカメラの位置を更新
				eye_ += normalizeDirection * remainingMoveDistance;
				remainingMoveDistance = 0.0f;
			}
		}

		worldTransform_.translation_ = eye_;

		//向きを更新する処理
		UpdateRotate();

		UpdateOffset();

	}

	worldTransform_.UpdateMatrix();

}

void RailMover::DrawRail(Camera* camera)
{
	//線の描画
	for (size_t i = 0; i < pointsDrawing_.size() - 1; i++) {
		Matrix4x4 point1 = MakeTranslateMatrix(pointsDrawing_[i]);
		Matrix4x4 point2 = MakeTranslateMatrix(pointsDrawing_[i + 1]);
		ModelPlatform::GetInstance()->LineDraw(point1, point2, camera);
	}
}

void RailMover::OnCollision(Collider* other)
{
	// WaveEventとの衝突時の処理
	if (WaveEvent* waveEvent = dynamic_cast<WaveEvent*>(other))
	{
		if (nextEnemyWaveNumber_ != waveEvent->GetWaveNumber())
		{
			return;
		}
		enemySpawnManager_->WaveStart(nextEnemyWaveNumber_);
		nextEnemyWaveNumber_++;
		
	}
	else if (SpeedEvent* speedEvent = dynamic_cast<SpeedEvent*>(other))
	{
		if (nextSpeedWaveNumber_ != speedEvent->GetWaveNumber())
		{
			return;
		}
		speed_ = speedEvent->GetSpeed();
		nextSpeedWaveNumber_++;
	}
}

void RailMover::CreateSplineCurve(const std::vector<Vector3>& controlPoints)
{
	// レベルデータから制御点を取得
	for (Vector3 controlPoint : controlPoints) {
		controlPoints_.push_back(controlPoint);
	}
	// Catmull-Romスプラインのポイントを生成
	pointsDrawing_ = GenerateCatmullRomSplinePoints(controlPoints_, segmentCount_);
}

void RailMover::UpdateRotate()
{
	//カメラの向きを更新する処理
	if (pointsDrawing_.size() > moveCount_ + difference_) {
		target_ = pointsDrawing_[moveCount_ + difference_];
		forward_ = Subtract(target_, worldTransform_.translation_);

		worldTransform_.rotation_ = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, forward_);

	}
}

void RailMover::UpdateOffset()
{
	//オフセットを更新する処理
	worldTransform_.UpdateMatrix();

	offset_ = { 0.0f, 3.0f, 0.0f };
	offset_ = TransformNormal(offset_, worldTransform_.worldMatrix_);
	worldTransform_.translation_ += offset_;
}