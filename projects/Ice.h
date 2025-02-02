#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"

class Ice
{
public:
	void Initialize(BaseModel* model_);
	void Update();
	void Draw();

private:
	WorldTransform worldTransform_;
	std::unique_ptr<Base3dObject> object_;
};

