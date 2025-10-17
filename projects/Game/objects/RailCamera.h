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

	//初期化
	void Initialize(Camera* camera, WorldTransform* parent);

	//更新
	void Update();

private:

	//カメラを更新
	void UpdateCamera();

	//ワールド変換データ
	WorldTransform worldTransform_;

	// カメラ
	Camera* camera_;

};