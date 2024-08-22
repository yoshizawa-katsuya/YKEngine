#pragma once
#include "Model.h"
class Camera;

class Player
{
public:

	void Initialize(Model* model, Camera* camera);

	void Update();

	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; }

private:

	//Transform変数を作る
	Transforms transform_{ {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f } };

	//カメラの変数
	Camera* camera_ = nullptr;

	Model* model_ = nullptr;

};

