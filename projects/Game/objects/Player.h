#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Animation.h"
class Camera;
class MapChipField;

class Player
{
public:

	void Initialize(BaseModel* model, Animation* animation);

	void Update();

	void Draw(Camera* camera);

private:

	//Transform変数を作る
	WorldTransform worldTransform_;

	std::unique_ptr<Base3dObject> object_;
	Animation* animation_;

	bool useAnimation_ = false;
};

