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
#include "GlobalVariables.h"
#include "JsonKeys.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void RailMover::Initialize(const std::vector<Vector3>& controlPoints, EnemySpawnManager* enemySpawnManager, bool isLoop)
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::RailMover::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::RailMover::kLerpAngleFactor, 0.1f);
	globalVariables->AddItem(groupName, JsonKey::RailMover::kRotateDuration, 0.5f);

	Collider::Initialize();
	typeID_ = CollisionTypeIdDef::kRailMover;

	enemySpawnManager_ = enemySpawnManager;
	isLoop_ = isLoop;

	CreateSplineCurve(controlPoints);
	// 初期位置をスプラインの最初のポイントに設定
	worldTransform_.translation_ = pointsDrawing_[0]; // 初期位置をスプラインの最初のポイントに設定

	//向きを初期化する処理
	InitializeRotate();

	worldTransform_.UpdateMatrix();

	srtAnimator_ = std::make_unique<SRTAnimator>();
}

void RailMover::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("RailMover");
	ImGui::Checkbox("DrawRail", &isDrawRail_);
	ImGui::End();

#endif // USE_IMGUI

	//曲線に沿って移動する処理
	if (pointsDrawing_.size() > moveCount_) 
	{
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

	//ループ処理
	if (isLoop_ && moveCount_ >= pointsDrawing_.size()) 
	{
		moveCount_ = 0;
	}

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
	// EnemySpawnEventTrigger、SpeedEventTrigger、RotateEventTrigger、RotateResetEventTriggerのいずれかと衝突した場合の処理
	if (EnemySpawnEventTrigger* enemySpawnEvent = dynamic_cast<EnemySpawnEventTrigger*>(other))
	{
		enemySpawnManager_->WaveStart(enemySpawnEvent->GetWaveNumber());
	}
	else if (SpeedEventTrigger* speedEvent = dynamic_cast<SpeedEventTrigger*>(other))
	{
		speed_ = speedEvent->GetSpeed();
	}
	else if (RotateEventTrigger* rotateEvent = dynamic_cast<RotateEventTrigger*>(other))
	{
		const float rotateDuration = GlobalVariables::GetInstance()->GetFloatValue(JsonKey::RailMover::kGroupName, JsonKey::RailMover::kRotateDuration);
		srtAnimator_->SetAnimation(worldTransform_.rotation_, rotateEvent->GetRotate(), rotateDuration);
		isInRotateEvent_ = true;
	}
	else if (RotateResetEventTrigger* rotateResetEvent = dynamic_cast<RotateResetEventTrigger*>(other))
	{
		isInRotateEvent_ = false;
	}
	
}

void RailMover::CreateSplineCurve(const std::vector<Vector3>& controlPoints)
{
	// レベルデータから制御点を取得
	for (Vector3 controlPoint : controlPoints) {
		controlPoints_.push_back(controlPoint);
	}
	// Catmull-Romスプラインのポイントを生成
	if (isLoop_) 
	{
		pointsDrawing_ = GenerateCatmullRomSplinePointsLoop(controlPoints_, kSegmentCount_);
		return;
	}
	pointsDrawing_ = GenerateCatmullRomSplinePoints(controlPoints_, kSegmentCount_);
}

void RailMover::InitializeRotate()
{
	target_ = pointsDrawing_[(moveCount_ + difference_) % pointsDrawing_.size()];
	forward_ = Subtract(target_, worldTransform_.translation_);
	worldTransform_.rotation_ = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, forward_);
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
	//通常の向き更新処理
	if (isLoop_) 
	{
		// ループしている場合、配列の範囲を超えたら最初に戻る
		target_ = pointsDrawing_[(moveCount_ + difference_) % pointsDrawing_.size()];
	}
	else if (moveCount_ + difference_ < pointsDrawing_.size()) 
	{
		// 通常のポイント取得
		target_ = pointsDrawing_[moveCount_ + difference_];
	}
	else
	{
		return; // 配列の範囲を超えた場合は処理を終了
	}
	forward_ = Subtract(target_, worldTransform_.translation_);
	Vector3 targetRotation = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, forward_);
	targetRotation.z = 0.0f; // Z軸回転を0に固定
	const float lerpAngleFactor = GlobalVariables::GetInstance()->GetFloatValue(JsonKey::RailMover::kGroupName, JsonKey::RailMover::kLerpAngleFactor);
	worldTransform_.rotation_ = LerpAngle(worldTransform_.rotation_, targetRotation, lerpAngleFactor);
}