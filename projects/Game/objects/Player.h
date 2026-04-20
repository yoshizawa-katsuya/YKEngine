#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "SRTAnimator.h"
#include "Animation.h"
#include "Input.h"
class YKEngine::Camera;

enum class PlayerPose{
	PoseBase,
	PoseSquat,
	PoseA,
	PoseB,
	PoseC,
	PoseD
};
enum class PlayerDirection{
	Left = 0,
	Front = 1,
	Right = 2
};
/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:

	void Initialize(YKEngine::BaseModel* model);

	void Update();

	void Draw(YKEngine::Camera* camera);

	PlayerPose GetPose()const { return pose_; }

	PlayerDirection GetDirection()const { return direction_; }

private:

	void ChangePose();

	void ChangeDirection();

	void UpdateColorForDebug();

private:

	//Transform変数を作る
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

	YKEngine::Input* input_ = nullptr;

	PlayerPose pose_;
	PlayerDirection direction_;

	const float kAngle45 = 3.141592f / 4.0f;
};

