#pragma once
#include "Model.h"
#include "WorldTransform.h"
class Camera;
class MapChipField;

class Player
{
public:

	void Initialize(Model* model);

	void Update();

	void Draw(Camera* camera);

private:

	//Transform変数を作る
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

};

