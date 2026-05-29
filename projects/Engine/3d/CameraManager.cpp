#include "CameraManager.h"
#include "Camera.h"
#include "WorldTransform.h"
#include <cmath>
#include <imgui.h>

/*
 CameraManager.cpp

 概要:
 - カメラの共有管理と「イージング（滑らかな回転遷移）」「ターゲットへの注視（LookAt）」など
   カメラ制御に関する補助的な機能を提供するモジュール。
 - シーン内で利用する Camera* を保持し、毎フレーム Update() で必要な補間処理を実行する。
 - 主な責務:
   * メインカメラの登録と毎フレーム更新
   * カメラ回転のイージング（easeTargetRotation_, easeFactor_ を用いた線形補間）
   * 指定座標へカメラを向ける LookAtTarget の計算（即時 or イージング）
   * カメラの簡易移動補助 MoveTargetAndCamera

 注意点:
 - 本クラスはスレッド非対応。メインスレッドから呼び出すことを想定。
 - 角度（ラジアン）の扱いは -π..π に正規化して最短経路で回転補間する。
 - easeFactor_ は 0..1 の範囲を期待する（範囲外はクランプする実装あり）。
 - LookAtTarget の pitch/yaw 計算は典型的な球面座標変換を利用している（gimbal lock の簡易対策は入れていない）。
*/
using namespace YKEngine;
using namespace CameraManagerConstants;

void CameraManager::Initialize(Camera* camera)
{
	// メインカメラを登録する
	mainCamera_ = camera;
}

void CameraManager::Update()
{
	// 毎フレーム呼ぶ更新処理
	if (mainCamera_ && isEasing_) {
		UpdateEasing();
	}

	// Camera 内で view/proj 行列を再計算
	if (mainCamera_) {
		mainCamera_->Update();
	}
}

void CameraManager::DrawImGui()
{
#ifdef USE_IMGUI
	ImGui::Begin("Camera Manager");

	if (mainCamera_) {
		DrawImGuiCameraTransform();

		// 行列表示
		DrawImGuiMatrix("World Matrix", mainCamera_->GetWorldMatrix());
		DrawImGuiMatrix("View Matrix", mainCamera_->GetViewMatrix());
		DrawImGuiMatrix("Projection Matrix", mainCamera_->GetProjection());
		DrawImGuiMatrix("View-Projection Matrix", mainCamera_->GetViewProjection());
	}

	ImGui::End();
#endif
}

void CameraManager::MoveTargetAndCamera(WorldTransform& target, const Vector3& delta)
{
	// カメラを移動
	if (mainCamera_) {
		Vector3 currentPos = mainCamera_->GetTranslate();
		mainCamera_->SetXTranslate(currentPos.x + delta.x);
	}
}

void CameraManager::LookAtTarget(const Vector3& targetPosition, bool eased, float easeFactor)
{
	if (!mainCamera_) return;

	// カメラから注視点への回転角を計算
	Vector3 camPos = mainCamera_->GetTranslate();
	Vector3 targetRotation = CalculateRotationToTarget(camPos, targetPosition);

	if (!eased) {
		// 即時適用
		mainCamera_->SetRotate(targetRotation);
		isEasing_ = false;
	}
	else {
		// イージング開始
		easeTargetRotation_ = targetRotation;
		easeFactor_ = ClampEaseFactor(easeFactor);
		isEasing_ = true;
	}
}

void CameraManager::SetCameraPosition(const Vector3& position)
{
	if (mainCamera_) {
		mainCamera_->SetTranslate(position);
	}
}

void CameraManager::SetCameraRotation(const Vector3& rotation)
{
	if (mainCamera_) {
		mainCamera_->SetRotate(rotation);
	}
}

void CameraManager::SetXPosition(float x)
{
	if (mainCamera_) {
		mainCamera_->SetXTranslate(x);
	}
}

// ===== ヘルパー関数 =====

void CameraManager::UpdateEasing()
{
	const Vector3 curRot = mainCamera_->GetRotate();

	// 現在の回転角
	float curPitch = curRot.x;
	float curYaw = curRot.y;

	// 目標との差を最短で取得
	float diffPitch = ShortestAngleDiff(curPitch, easeTargetRotation_.x);
	float diffYaw = ShortestAngleDiff(curYaw, easeTargetRotation_.y);

	// 線形補間で新しい回転を計算
	float newPitch = curPitch + diffPitch * easeFactor_;
	float newYaw = curYaw + diffYaw * easeFactor_;

	// 収束判定
	if (IsEasingComplete(diffPitch, diffYaw)) {
		isEasing_ = false;
		newPitch = easeTargetRotation_.x;
		newYaw = easeTargetRotation_.y;
	}

	// Z（ロール）は 0 に固定
	mainCamera_->SetRotate(Vector3(newPitch, newYaw, kDefaultRotationZ));
}

float CameraManager::NormalizeAngle(float angle)
{
	// 角度を (-PI, PI] の範囲に正規化
	while (angle > kPI) {
		angle -= kTwoPI;
	}
	while (angle <= -kPI) {
		angle += kTwoPI;
	}
	return angle;
}

float CameraManager::ShortestAngleDiff(float current, float target)
{
	// 最短角度差を計算
	float diff = NormalizeAngle(target - current);
	return diff;
}

Vector3 CameraManager::CalculateRotationToTarget(const Vector3& fromPosition, const Vector3& toPosition)
{
	// カメラから注視点への方向ベクトル
	Vector3 direction = toPosition - fromPosition;

	// 回転角の計算（ラジアン）
	float yaw = std::atan2f(direction.x, direction.z);
	float horizontalDistance = std::sqrtf(direction.x * direction.x + direction.z * direction.z);
	float pitch = -std::atan2f(direction.y, horizontalDistance);

	return Vector3(pitch, yaw, kDefaultRotationZ);
}

float CameraManager::ClampEaseFactor(float factor)
{
	// イージング係数を有効範囲にクランプ
	if (factor < kMinEaseFactor) return kMinEaseFactor;
	if (factor > kMaxEaseFactor) return kMaxEaseFactor;
	return factor;
}

bool CameraManager::IsEasingComplete(float diffPitch, float diffYaw) const
{
	// 目標に十分近づいたかを判定
	return std::fabs(diffPitch) < angleEpsilon_ && std::fabs(diffYaw) < angleEpsilon_;
}

void CameraManager::DrawImGuiCameraTransform()
{
	Vector3 camPos = mainCamera_->GetTranslate();
	Vector3 camRot = mainCamera_->GetRotate();
	float camFovY = mainCamera_->GetFovY();
#ifdef USE_IMGUI

	if (ImGui::DragFloat3("Camera Position", &camPos.x)) {
		mainCamera_->SetTranslate(camPos);
	}

	if (ImGui::SliderFloat3("Camera Rotation", &camRot.x, 3.14f, 0.1f)) {
		mainCamera_->SetRotate(camRot);
	}

	if (ImGui::DragFloat("Camera FovY", &camFovY, kFovYStep, kMinFovY, kMaxFovY)) {
		mainCamera_->SetFovY(camFovY);
	}
#endif
}

void CameraManager::DrawImGuiMatrix(const char* label, const Matrix4x4& matrix)
{
#ifdef USE_IMGUI
	ImGui::Text("%s:", label);
	for (int i = 0; i < kMatrixRows; ++i) {
		ImGui::Text("| %.2f %.2f %.2f %.2f |",
			matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
	}
#endif
}
