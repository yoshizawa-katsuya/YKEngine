#pragma once
#include "BaseModel.h"
#include "SkinModel.h"
#include "WorldTransform.h"

class Ground
{


public:

	void Initialize(BaseModel* model);

	void Update();

	void Draw(Camera* camera);

private:

	WorldTransform worldTransform_;

	BaseModel* model_ = nullptr;

};

