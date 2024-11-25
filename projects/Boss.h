#pragma once
#pragma once
#include "BaseModel.h"
#include "Base3dObject.h"
#include "SkinModel.h"
#include "WorldTransform.h"
#include "Animation.h"
#include <memory>
class Camera;
class PlayerLockOn;

class Boss
{

public:

	void Initialize(BaseModel* model);

	void Update();

	void Draw(Camera* camera);

	void SetLockOn(PlayerLockOn* lockOn) { lockOn_ = lockOn; }

private:

	//Transform変数を作る
	WorldTransform worldTransform_;

	std::unique_ptr<Base3dObject> object_;
	// 借りてくる
	PlayerLockOn* lockOn_ = nullptr;

	

};


