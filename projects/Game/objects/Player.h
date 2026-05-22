#pragma once
#include "My3dObject.h"
#include "Skin3dObject.h"
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

	void PlayAnimation(const std::string& name);

	/// <summary> ポーズの切り替わりを検知し、適切なアニメーションを開始する </summary>
	void UpdateAnimationTrigger();

	/// <summary> Returnモーションなどの時間経過による自動遷移を管理する </summary>
	void UpdateAnimationTimers();

private:

	//Transform変数を作る
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::Skin3dObject> object_;
	std::map<std::string, std::unique_ptr<YKEngine::Animation>> animations_;
	YKEngine::Animation* currentAnimation_ = nullptr;
	std::string currentAnimationName_ = "";

	YKEngine::Input* input_ = nullptr;

	PlayerPose pose_;
	PlayerPose prevPose_;

	bool isReturnPhase_ = false;
	float returnTimer_ = 0.0f;

	PlayerDirection direction_;
	float kAngle_;
};