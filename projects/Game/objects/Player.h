#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "SRTAnimator.h"
#include "Animation.h"
#include "Input.h"
#include "GameType.h"
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

	const YKEngine::WorldTransform& GetWorldTransform()const { return worldTransform_; }

	PlayerPose GetPose()const { return pose_; }

	PlayerDirection GetDirection()const { return direction_; }

	PoseDir GetState()const { return { pose_,direction_ }; }

	void SetColorForDebug(YKEngine::Vector4& color)const;

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

	float kAngle_;
};

