#pragma once
#include "Rigid3dObject.h"
#include "WorldTransform.h"
#include "SRTAnimator.h"
#include "Animation.h"
class Camera;
class MapChipField;

class Player
{
public:

	void Initialize(BaseModel* model);

	void Update();

	void Draw(Camera* camera);

	void SetWorldTransform(const WorldTransform& worldTransform) { worldTransform_ = worldTransform; }

private:

	//Transform変数を作る
	WorldTransform worldTransform_;

	std::unique_ptr<Rigid3dObject> object_;

};

