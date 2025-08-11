#pragma once
#include "WorldTransform.h"
#include "Camera.h"

//レールカメラ
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