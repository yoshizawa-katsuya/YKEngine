#pragma once
#include "BaseModel.h"
#include "WorldTransform.h"
class Camera;
class MapChipField;

class Player
{
public:

	void Initialize(BaseModel* model);

	void Update();

	void Draw(Camera* camera);

private:

	//Transform変数を作る
	WorldTransform worldTransform_;

	BaseModel* model_ = nullptr;

};

