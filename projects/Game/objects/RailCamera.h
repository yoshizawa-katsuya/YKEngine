#pragma once
#include "WorldTransform.h"
#include "Camera.h"
class Player;

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
	void Initialize(Camera* camera, WorldTransform* parent, WorldTransform* playerWorldTransform);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// ゲームオーバーしたことを通知。
	/// </summary>
	void SetGameOver();

	/// <summary>
	/// 自機の逆ローカル方向からtargetRotation_を作成。
	/// </summary>
	void CreateTargetRotationFromPlayer(const Vector3& direction);

private:

	/// <summary>
	/// メインフェーズの更新。
	/// </summary>
	void UpdateMain();

	/// <summary>
	/// ゲームオーバーフェーズの更新。
	/// </summary>
	void UpdateGameOver();

	enum class Phase
	{
		Main,	// メイン
		GameOver,	// ゲームオーバー
	};
	Phase phase_ = Phase::Main;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//自機を親とするワールド変換データ
	WorldTransform playerParentWorldTransform_;

	// 自機を注視する際の係数
	float t_ = 0.0f;

	// カメラ
	Camera* camera_;

	Vector3 targetRotation_{};

};