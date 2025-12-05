#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "SRTAnimator.h"
#include "Animation.h"
class YKEngine::Camera;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:

	void Initialize(YKEngine::BaseModel* model);

	void Update();

	void Draw(YKEngine::Camera* camera);

private:

	//Transform変数を作る
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

};

