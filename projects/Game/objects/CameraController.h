#pragma once
#include "Struct.h"
#include "Camera.h"
class Player;

class CameraController
{
public:

	/// <summary>
	/// 初期化
	///  </summary>
	void Initialize(Camera* camera, Player* target);

	/// <summary>
	/// 更新
	///  </summary>
	void Update();

private:

	// 矩形
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	//カメラ移動範囲
	Rect movableArea_ = { -15.0f, 15.0f, 6.0f, 6.0f };

	//追従対象の各方向へのカメラ移動範囲
	Rect margin_ = { -8.0f, 8.0f, -5.0f, 5.0f };

	Camera* camera_;

	Player* target_ = nullptr;

	//追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = { 0, 4.0f, -15.0f };

	//カメラの目標座標
	Vector3 targetPosition_;

	//座標補完割合
	float kInterpolationRate = 0.2f;

	//速度掛け率
	//float kVelocityBies = 5.0f;
};

