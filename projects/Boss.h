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

	void Initialize(const std::vector<BaseModel*>& models);

	void Update();

	void Draw(Camera* camera);

private:

	//Transform変数を作る
	std::vector<WorldTransform> worldTransforms_;

	std::vector<BaseModel*> models_;

};


