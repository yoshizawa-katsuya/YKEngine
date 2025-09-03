#pragma once
#include "Rigid3dObject.h"
#include "WorldTransform.h"
#include "SRTAnimator.h"
#include "Animation.h"

class Camera;
class MapChipField;

class BaseCharacter
{
public: // 公開メンバ関数

	virtual void Initialize(BaseModel* model);

	virtual void Update();

	virtual void Draw(Camera* camera);

protected:

	//Transform変数を作る
	WorldTransform worldTransform_;

	std::unique_ptr<Rigid3dObject> object_;
};

