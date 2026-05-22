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

	void RequestDeath() { requestDeath_ = true; }

	bool IsDead() const { return isDead_; }

	bool IsDeathFinished() const { return isDeathFinished_; }

	void SetColorForDebug(YKEngine::Vector4& color)const;

	void Reset();

private:

	void ChangePose();

	void ChangeDirection();

	void UpdateColorForDebug();

	// 死亡開始関数
	void StartDeathAnimation();

	// 死亡アニメーション
	void PlayDeathAnimation();

	enum class DeathVariation
	{
		Right,
		Left,
		InFront
	};
	DeathVariation deathVariation_ = DeathVariation::Right;

	enum class DeathPhase
	{
		FlyUp,
		FlyDown,
		Finished
	};
	DeathPhase deathPhase_ = DeathPhase::FlyUp;

private:

	//Transform変数を作る
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

	YKEngine::Input* input_ = nullptr;

	PlayerPose pose_;
	PlayerDirection direction_;

	float kAngle_;

	// 死亡開始要求
	bool requestDeath_ = false;

	// 死亡中か
	bool isDead_ = false;

	// 演出終了
	bool isDeathFinished_ = false;

	// 吹っ飛び速度
	YKEngine::Vector3 deathVelocity_ = {};

	YKEngine::Vector3 kRightDeathVelocity = { 1.0f, 1.0f, -0.85f };

	YKEngine::Vector3 kLeftDeathVelocity = { -1.0f, 1.0f, -0.85f };

	YKEngine::Vector3 kFrontDeathVelocity = { 0.0f, 0.08f, -0.85f };

	// 回転速度
	YKEngine::Vector3 deathRotateVelocity_ = {};

	// 演出タイマー
	float deathTimer_ = 0.0f;

	// 開始位置・回転・スケール
	YKEngine::Vector3 startPosition_;
	YKEngine::Vector3 startRotation_;
	YKEngine::Vector3 startScale_;
};

