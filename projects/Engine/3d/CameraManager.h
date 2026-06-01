#pragma once
#include <Camera.h>
#include <WorldTransform.h>
#include <Vector3.h>

using namespace YKEngine;

// CameraManager用の定数
namespace CameraManagerConstants {
	// デフォルトのイージング係数
	constexpr float kDefaultEaseFactor = 0.1f;

	// 角度のイプシロン（収束判定用）
	constexpr float kAngleEpsilon = 1e-3f;

	// イージング係数の範囲
	constexpr float kMinEaseFactor = 0.0f;
	constexpr float kMaxEaseFactor = 1.0f;

	// デフォルトの回転値
	constexpr float kDefaultRotationX = 0.0f;
	constexpr float kDefaultRotationY = 0.0f;
	constexpr float kDefaultRotationZ = 0.0f;

	// 円周率
	constexpr float kPI = 3.14159265358979323846f;
	constexpr float kTwoPI = 2.0f * kPI;

	// ImGuiの行列表示用
	constexpr int kMatrixRows = 4;
	constexpr int kMatrixColumns = 4;

	// FovYの調整範囲
	constexpr float kMinFovY = 0.1f;
	constexpr float kMaxFovY = 3.14f;
	constexpr float kFovYStep = 0.01f;
}

/// <summary>
/// カメラ管理クラス
/// </summary>
class CameraManager
{
public:
	// 初期化
	void Initialize(Camera* camera);

	// 更新
	void Update();

	// ImGui描画
	void DrawImGui();

	// 対象とカメラを一緒に移動させる
	void MoveTargetAndCamera(WorldTransform& target, const Vector3& delta);

	// カメラが対象に向き続ける
	void LookAtTarget(const Vector3& targetPosition, bool eased = false, float easeFactor = CameraManagerConstants::kDefaultEaseFactor);

	// セッター
	void SetMainCamera(Camera* camera) { mainCamera_ = camera; }
	void SetCameraPosition(const Vector3& position);
	void SetCameraRotation(const Vector3& rotation);
	void SetXPosition(float x);

	// ゲッター
	Camera* GetMainCamera() const { return mainCamera_; }
	bool IsEasing() const { return isEasing_; }
	const Vector3& GetEaseTargetRotation() const { return easeTargetRotation_; }
	float GetEaseFactor() const { return easeFactor_; }

private:
	// イージング更新
	void UpdateEasing();

	// 角度の正規化
	static float NormalizeAngle(float angle);

	// 最短角度差の計算
	static float ShortestAngleDiff(float current, float target);

	// 回転角の計算
	static Vector3 CalculateRotationToTarget(const Vector3& fromPosition, const Vector3& toPosition);

	// イージング係数のクランプ
	static float ClampEaseFactor(float factor);

	// イージング完了判定
	bool IsEasingComplete(float diffPitch, float diffYaw) const;

	// ImGuiヘルパー
	void DrawImGuiCameraTransform();
	void DrawImGuiMatrix(const char* label, const Matrix4x4& matrix);

	// メインカメラ
	Camera* mainCamera_ = nullptr;

	// イージング用
	bool isEasing_ = false;
	Vector3 easeTargetRotation_ = {
		CameraManagerConstants::kDefaultRotationX,
		CameraManagerConstants::kDefaultRotationY,
		CameraManagerConstants::kDefaultRotationZ
	};
	float easeFactor_ = CameraManagerConstants::kDefaultEaseFactor;
	float angleEpsilon_ = CameraManagerConstants::kAngleEpsilon;
};


