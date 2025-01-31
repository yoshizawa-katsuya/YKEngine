#pragma once
#include "Camera.h"
#include <WorldTransform.h>
class LockOn;

///<summary>
///追従カメラ
///</summary>
class FollowCamera {
public:

	//初期化
	void Initialize(Camera* camera);

	void Reset();

	//更新
	void Update();

	//追従対象の設定
	void SetTarget(const WorldTransform* target);

	//オフセット計算
	Vector3 OffsetCalculation();

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

private:

	//カメラ
	Camera* camera_;
	
	//追従対象
	const WorldTransform* target_ = nullptr;

	//追従対象の残像座標
	Vector3 interTarget_ = {};

	//目標角度
	float destinationAngleY_;

	//ロックオン
	const LockOn* lockOn_ = nullptr;

};