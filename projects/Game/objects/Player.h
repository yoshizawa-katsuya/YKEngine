#pragma once
#include "Model.h"
#include "Animation.h"
class Camera;


class Player
{
public:

	void Initialize(Model* model);

	void Update();

	void Draw(Camera* camera);

private:

	//Transform変数を作る
	EulerTransform transform_{ {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f } };

	Model* model_ = nullptr;

};

