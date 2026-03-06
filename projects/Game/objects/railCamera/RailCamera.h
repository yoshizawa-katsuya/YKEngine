#pragma once
#include "WorldTransform.h"
#include <memory>
#include "StateMachine.hpp"
#include "RailCameraStateContext.h"

namespace YKEngine
{
	class Camera;
}

/// <summary>
/// レールカメラ。
/// RailMoverを追従するカメラ。
/// カメラの位置はRailMoverに依存する。
/// </summary>
class RailCamera : private RailCameraStateContext
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="camera">レールカメラとして使用するカメラ。</param>
	/// <param name="parent">親ワールド変換データ。</param>
	void Initialize(YKEngine::WorldTransform* parent, YKEngine::WorldTransform* playerWorldTransform);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// ゲームオーバーしたことを通知。
	/// </summary>
	void SetGameOver() { isGameOver_ = true; }

	/// <summary>
	/// クリアシーンに移行したことを通知。
	/// </summary>
	void SetClearScene();

	/// <summary>
	/// スタート演出に移行したことを通知。
	/// </summary>
	void SetStart();

	/// <summary>
	/// 方向からtargetRotation_を作成。
	/// </summary>
	void CreateTargetRotationFromDirection(const YKEngine::Vector3& direction);

	YKEngine::Camera* GetCamera() { return camera_.get(); }

private:

	/// <summary>
	/// メインの更新。
	/// </summary>
	void UpdateStart() override;

	/// <summary>
	/// メインフェーズの更新。
	/// </summary>
	void UpdateMain() override;

	/// <summary>
	/// ゲームオーバーフェーズの更新。
	/// </summary>
	void UpdateGameOver() override;

	/// <summary>
	/// クリアシーンフェーズの更新。
	/// </summary>
	void UpdateClearScene() override;

	/// <summary>
	/// クリアシーンステートに入る際の処理
	/// </summary>
	void EnterClearScene() override;

	/// <summary>
	/// ゲームオーバーならtrue
	/// </summary>
	bool IsGameOver() const override { return isGameOver_; }

	/// <summary>
	/// 補完係数t_をリセット
	/// </summary>
	void ReaetT() override { t_ = 0.0f; }

	/// <summary>
	/// 補完係数t_を取得
	/// </summary>
	float GetT() const override { return t_; }

	// ステートマシン
	std::unique_ptr<YKEngine::StateMachine<RailCameraStateContext>> stateMachine_;

	//ワールド変換データ
	YKEngine::WorldTransform worldTransform_;

	//目標を注視するためのワールド変換データ
	YKEngine::WorldTransform targetParentWorldTransform_;

	// 自機を注視する際の係数
	float t_ = 0.0f;

	// カメラ
	std::unique_ptr<YKEngine::Camera> camera_;

	YKEngine::Vector3 targetRotation_{};

	//ゲームオーバーならtrue
	bool isGameOver_ = false;
};