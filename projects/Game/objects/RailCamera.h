#pragma once
#include "WorldTransform.h"
#include <memory>

namespace YKEngine
{
	class Camera;
}

/// <summary>
/// レールカメラ。
/// RailMoverを追従するカメラ。
/// カメラの位置はRailMoverに依存する。
/// </summary>
class RailCamera {
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
	void SetGameOver();

	/// <summary>
	/// クリアシーンに移行したことを通知。
	/// </summary>
	void SetClearScene();

	/// <summary>
	/// 方向からtargetRotation_を作成。
	/// </summary>
	void CreateTargetRotationFromDirection(const YKEngine::Vector3& direction);

	YKEngine::Camera* GetCamera() { return camera_.get(); }

private:

	/// <summary>
	/// メインフェーズの更新。
	/// </summary>
	void UpdateMain();

	/// <summary>
	/// ゲームオーバーフェーズの更新。
	/// </summary>
	void UpdateGameOver();

	/// <summary>
	/// クリアシーンフェーズの更新。
	/// </summary>
	void UpdateClearScene();

	enum class Phase
	{
		kMain,	// メイン
		kGameOver,	// ゲームオーバー
		kClearScene,	// クリアシーン
	};
	Phase phase_ = Phase::kMain;

	//ワールド変換データ
	YKEngine::WorldTransform worldTransform_;

	//目標を注視するためのワールド変換データ
	YKEngine::WorldTransform targetParentWorldTransform_;

	// 自機を注視する際の係数
	float t_ = 0.0f;

	// カメラ
	std::unique_ptr<YKEngine::Camera> camera_;

	YKEngine::Vector3 targetRotation_{};

};