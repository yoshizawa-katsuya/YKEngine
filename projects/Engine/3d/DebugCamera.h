#pragma once
#include "Struct.h"
class Camera;
class Input;


class DebugCamera
{
public:

	//初期化
	void Initialize(Camera* camera, Input* input);

	//更新
	void Update();

	void ViewMatrixUpdate();

	void AddRotateX(float rotateX);

	void AddRotateY(float rotateY);

	void AddRotateZ(float rotateZ);

private:

	Camera* camera_;
	Input* input_;

	Matrix4x4 matRot_;

	Vector3 target_ = { 0.0f, 0.0f, 10.0f };

};

