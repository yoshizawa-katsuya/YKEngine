#pragma once
#pragma once
#include "BaseModel.h"
#include "SkinModel.h"
#include "WorldTransform.h"
#include "Animation.h"
class Camera;

class Boss
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


