#pragma once
#include "WorldTransform.h"
#include "Camera.h"

//レールカメラ
class RailCamera {
public:

	//初期化
	void Initialize(const Vector3 worldPos, const Vector3 rotate, Camera* camera);

	//更新
	void Update();

	Camera* GetCamera() { return camera_; }

	WorldTransform* GetWorldTransform() { return &worldTransform_; }

private:
	
	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	Camera* camera_;

};