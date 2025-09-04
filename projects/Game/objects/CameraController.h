#pragma once
#include "WorldTransform.h"
class Camera;
class Player;
class MapChipField;

/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController {

public:

	// 矩形
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	/// <summary>
	/// 初期化
	///  </summary>
	void Initialize(Camera* camera, Player* target, MapChipField* mapChipField);

	/// <summary>
	/// 更新
	///  </summary>
	void Update();

	void Reset();

	void SetTarget(Player* target) { target_ = target; };

private:
	
	Camera* camera_ = nullptr;

	EulerTransform transform_;

	//カメラ移動範囲
	Rect movableArea_ = { 11, 187, 6, 32 };

	//追従対象の各方向へのカメラ移動範囲
	const Rect margin_ = {-8.0f, 8.0f, -5.0f, 5.0f};

	Player* target_ = nullptr;

	//追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -30.0f};

	//カメラの目標座標
	Vector3 targetPosition_;

	//座標補完割合
	const float kInterpolationRate = 0.3f;

	//速度掛け率
	const float kVelocityBies = 5.0f;

};