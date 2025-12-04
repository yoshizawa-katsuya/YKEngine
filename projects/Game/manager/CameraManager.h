#pragma once
#include <memory>
#include "RailCamera.h"
#include "DebugCamera.h"
#include "Camera.h"
class ModelPlatform;

class CameraManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="railMoverWorldTransform">レールムーバーのワールド変換</param>
	/// <param name="playerWorldTransform">プレイヤーのワールド変換</param>
	void Initialize(WorldTransform* railMoverWorldTransform, WorldTransform* playerWorldTransform);

	/// <summary>
	/// クリアシーン用初期化
	/// </summary>
	/// <param name="railMoverWorldTransform">レールムーバーのワールド変換</param>
	/// <param name="targetDirection">注視点への方向ベクトル</param>
	void InitializeForClearScene(WorldTransform* railMoverWorldTransform, const Vector3& targetDirection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// レールカメラの更新	
	/// </summary>
	void UpdateRailCamera();

	/// <summary>
	/// デバッグカメラの更新
	/// </summary>
	/// <param name="targetDirection">注視点への方向ベクトル</param>	
	void UpdateRailCamera(const Vector3& targetDirection);

	/// <summary>
	/// ゲームオーバー時の処理
	/// </summary>
	/// <param name="targetDirection">注視点への方向ベクトル</param>
	void ProcessGameOver(const Vector3& targetDirection);

	RailCamera* GetRailCamera() { return railCamera_.get(); }
	
	Camera* GetRailCameraInner() { return railCamera_->GetCamera(); }

	DebugCamera* GetDebugCamera() { return debugCamera_.get(); }

	Camera* GetMainCamera() { return mainCamera_; }

private:

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;

	//メインカメラ
	Camera* mainCamera_ = nullptr;

	bool isActiveDebugCamera_ = false;

	//モデルプラットフォーム
	ModelPlatform* modelPlatform_ = nullptr;
};

