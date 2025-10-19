#pragma once
#include "WorldTransform.h"
#include "Camera.h"

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
	void Initialize(Camera* camera, WorldTransform* parent);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

private:

	/// <summary>
	/// カメラの更新。
	/// </summary>
	void UpdateCamera();

	//ワールド変換データ
	WorldTransform worldTransform_;

	// カメラ
	Camera* camera_;

};