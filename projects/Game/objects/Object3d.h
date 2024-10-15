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

	void SetTransform(const WorldTransform& transform) { worldTransform_ = transform; }

private:

	//Transform変数を作る
	WorldTransform worldTransform_;

	//カメラの変数
	//Camera* camera_ = nullptr;

	Model* model_ = nullptr;

};

