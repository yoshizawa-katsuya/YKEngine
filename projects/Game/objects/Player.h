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

	void BoneDraw(Camera* camera);

	void JointDraw(Camera* camera);

	//void SetCamera(Camera* camera) { camera_ = camera; }

	void SetAnimation(Animation* animation) { animation_ = animation; }

private:

	//Transform変数を作る
	EulerTransform transform_{ {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f } };

	//カメラの変数
	//Camera* camera_ = nullptr;

	Model* model_ = nullptr;

	Animation* animation_ = nullptr;

};

