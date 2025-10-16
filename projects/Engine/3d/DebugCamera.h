#pragma once
#include "Struct.h"
class Camera;
class Input;

/// <summary>
/// デバッグ用カメラ。
/// マウスで操作可能。
/// </summary>
class DebugCamera
{
public:

	//初期化
	void Initialize(Camera* camera, Input* input);

	//更新
	void Update();

private:

	void ViewMatrixUpdate();

	Camera* camera_;
	Input* input_;

	Matrix4x4 matRot_;

	Vector3 target_ = { 0.0f, 0.0f, 10.0f };

};

