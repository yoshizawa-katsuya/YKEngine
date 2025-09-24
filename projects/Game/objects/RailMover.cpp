#include "RailMover.h"
#include "Curve.h"
#include "TransformHelpers.h"
#include "Matrix.h"
#include "ModelPlatform.h"
#include "eventTrigger/EnemySpawnEventTrigger.h"
#include "manager/EnemySpawnManager.h"
#include "eventTrigger/SpeedEventTrigger.h"
#include "eventTrigger/RotateEventTrigger.h"
#include "eventTrigger/RotateResetEventTrigger.h"
#include "imgui/imgui.h"

void RailMover::Initialize(const std::vector<Vector3>& controlPoints, EnemySpawnManager* enemySpawnManager)
{
	Collider::Initialize();
	typeID_ = CollisionTypeIdDef::kRailMover;

	enemySpawnManager_ = enemySpawnManager;

	CreateSplineCurve(controlPoints);
	// 初期位置をスプラインの最初のポイントに設定
	worldTransform_.translation_ = pointsDrawing_[0]; // 初期位置をスプラインの最初のポイントに設定

	//向きを更新する処理
	UpdateRotate();

	worldTransform_.UpdateMatrix();

	srtAnimator_ = std::make_unique<SRTAnimator>();
}

void RailMover::Update()
{
#ifdef _DEBUG

	ImGui::Begin("RailMover");
	ImGui::Checkbox("DrawRail", &isDrawRail_);
	ImGui::End();

#endif // _DEBUG

	if (pointsDrawing_.size() > moveCount_) {
		//残りの移動距離計算用の変数
		float remainingMoveDistance = speed_;

		// 位置を更新する処理
		while (remainingMoveDistance > 0.0f && pointsDrawing_.size() > moveCount_)
		{
			Vector3 moveDirection = pointsDrawing_[moveCount_] - worldTransform_.translation_;
			float distance = Length(moveDirection);

			// ほぼゼロ距離の場合はスキップ
			if (distance < 0.001f) {
				moveCount_++;
				continue;
			}

			if (distance < remainingMoveDistance)
			{
				// 次のポイントに移動する距離が残りの距離よりも短い場合、次のポイントに移動
				worldTransform_.translation_ = pointsDrawing_[moveCount_];
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


		//向きを更新する処理
		UpdateRotate();

	}

	worldTransform_.UpdateMatrix();

}

void RailMover::DrawRail(Camera* camera)
{
	if (isDrawRail_) 
	{
		//線の描画
		for (size_t i = 0; i < pointsDrawing_.size() - 1; i++) {
			Matrix4x4 point1 = MakeTranslateMatrix(pointsDrawing_[i]);
			Matrix4x4 point2 = MakeTranslateMatrix(pointsDrawing_[i + 1]);
			ModelPlatform::GetInstance()->LineDraw(point1, point2, camera);
		}
	}
	
}

void RailMover::OnCollision(Collider* other)
{
	// WaveEventとの衝突時の処理
	if (EnemySpawnEventTrigger* waveEvent = dynamic_cast<EnemySpawnEventTrigger*>(other))
	{
		if (nextEnemySpawnEventNumber_ != waveEvent->GetWaveNumber())
		{
			return;
		}
		enemySpawnManager_->WaveStart(nextEnemySpawnEventNumber_);
		nextEnemySpawnEventNumber_++;
		
	}
	else if (SpeedEventTrigger* speedEvent = dynamic_cast<SpeedEventTrigger*>(other))
	{
		if (nextSpeedEventNumber_ != speedEvent->GetWaveNumber())
		{
			return;
		}
		speed_ = speedEvent->GetSpeed();
		nextSpeedEventNumber_++;
	}
	else if (RotateEventTrigger* rotateEvent = dynamic_cast<RotateEventTrigger*>(other))
	{
		if (nextRotateEventNumber_ != rotateEvent->GetWaveNumber())
		{
			return;
		}
		srtAnimator_->SetAnimation(worldTransform_.rotation_, rotateEvent->GetRotate(), 0.5f);
		isInRotateEvent_ = true;
		nextRotateEventNumber_++;
	}
	else if (RotateResetEventTrigger* rotateResetEvent = dynamic_cast<RotateResetEventTrigger*>(other))
	{
		if (nextRotateResetEventNumber_ != rotateResetEvent->GetWaveNumber())
		{
			return;
		}
		isInRotateEvent_ = false;
		nextRotateResetEventNumber_++;
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
	if (isInRotateEvent_) 
	{
		// 回転イベント中は回転アニメーションを適用
		worldTransform_.rotation_ = srtAnimator_->Update();
		return;
	}
	else if (pointsDrawing_.size() > moveCount_ + difference_)
	{
		target_ = pointsDrawing_[moveCount_ + difference_];
		forward_ = Subtract(target_, worldTransform_.translation_);

		Vector3 targetRotation = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, forward_);
		worldTransform_.rotation_ = Lerp(worldTransform_.rotation_, targetRotation, 0.1f);

	}
}