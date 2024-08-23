#pragma once
#include "Model.h"
class Camera;

class Object3d
{
public:

	void Initialize(Model* model);

	void Update(const std::string& label);

	void Draw(Camera* camera);

	//void SetCamera(Camera* camera) { camera_ = camera; }

	void SetTransform(const Transforms& transform) { transform_ = transform; }

private:

	//Transform変数を作る
	Transforms transform_{ {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f } };

	//カメラの変数
	//Camera* camera_ = nullptr;

	Model* model_ = nullptr;

};

