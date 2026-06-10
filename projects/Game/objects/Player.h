#pragma once
#include "My3dObject.h"
#include "Skin3dObject.h"
#include "WorldTransform.h"
#include "SRTAnimator.h"
#include "Animation.h"
#include "Input.h"
#include "GameType.h"
#include "ParticleEmitter.h"
#include "Audio.h"
class YKEngine::Camera;

enum class DeathVariation
{
	Right,
	Left,
	InFront
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

	const YKEngine::WorldTransform& GetWorldTransform()const { return worldTransform_; }

	PlayerPose GetPose()const { return pose_; }

	PlayerDirection GetDirection()const { return direction_; }

	PoseDir GetState()const { return { pose_,direction_ }; }

	float GetHitStopTimer() const { return hitStopTimer_; }

	DeathVariation GetDeathVariation() const { return deathVariation_; }

	void RequestDeath() { requestDeath_ = true; }

	bool ConsumeResetRequest();

	bool IsInHitImpact() const { return state_ == PlayerState::HitImpact; }

	bool IsDead() const { return state_ == PlayerState::Dead; }

	bool IsAlive() const { return state_ == PlayerState::Normal; }

	bool IsDeathFinished() const { return state_ == PlayerState::DeadFinished; }

	void SetColorForDebug(YKEngine::Vector4& color)const;

	void Reset();

	void SetAutoPoseDemo(bool enable) { isAutoPoseDemo_ = enable; }

	void SetPositon(YKEngine::Vector3 pos) { worldTransform_.translation_ = pos; }

	void SetRotate(YKEngine::Vector3 rot) { worldTransform_.rotation_ = rot; }

	void SetDirectionControl(bool enable) { isDirectionControl_ = enable; }

	void SetPoseControl(bool enable) { isPoseControl_ = enable; }

	void ChangeAnimation(const std::string& name);

	void StartDamageReaction();

private:

	void ChangePose();

	void ChangeDirection();

	void UpdateColorForDebug();

	void PlayAnimation(const std::string& name);

	/// <summary> ポーズの切り替わりを検知し、適切なアニメーションを開始する </summary>
	void UpdateAnimationTrigger();

	/// <summary> Returnモーションなどの時間経過による自動遷移を管理する </summary>
	void UpdateAnimationTimers();

	// 死亡開始関数
	void StartDeathAnimation();

	// 死亡アニメーション
	void PlayDeathAnimation();

	DeathVariation deathVariation_ = DeathVariation::Right;

	enum class PlayerState
	{
		Normal,
		HitImpact,      // ←追加
		Dead,
		DeadFinished
	};

	PlayerState state_ = PlayerState::Normal;

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
	bool isPoseControl_ = true;

	bool isReturnPhase_ = false;
	float returnTimer_ = 0.0f;

	PlayerDirection direction_;
	float kAngle_;
	bool isDirectionControl_ = true;

	// 死亡開始要求
	bool requestDeath_ = false;

	// リセット要求
	bool resetRequested_ = false;

	// プレイヤーを表示するか
	bool isVisible_ = true;

	std::unique_ptr<YKEngine::ParticleEmitter> deathEmitter_;

	std::unique_ptr<YKEngine::ParticleEmitter> ringEmitter_;

	std::shared_ptr<YKEngine::BaseModel> particleModel_;

	std::shared_ptr<YKEngine::BaseModel> ringParticleModel_;

	uint32_t textureHandle;

	uint32_t ringTextureHandle;

	// 吹っ飛び速度
	YKEngine::Vector3 deathVelocity_ = {};

	YKEngine::Vector3 kRightDeathVelocity = { 1.0f, 1.0f, -0.85f };

	YKEngine::Vector3 kLeftDeathVelocity = { -1.0f, 1.0f, -0.85f };

	YKEngine::Vector3 kFrontDeathVelocity = { 0.0f, 0.5f, -1.6f };

	// 回転速度
	YKEngine::Vector3 // グルグル回転
		deathRotateVelocity_ =
	{
		0.45f,
		0.05f,
		0.0f
	};

	// 演出タイマー
	float deathTimer_ = 0.0f;

	// 開始位置・回転・スケール
	YKEngine::Vector3 startPosition_;
	YKEngine::Vector3 startRotation_;
	YKEngine::Vector3 startScale_;

	// ヒットストップ時間
	float hitStopTimer_ = 0.0f;

	// ヒットストップ時間の長さ
	const float kHitStopTime_ = 0.75f;

	// ダメージフラッシュ中かどうか
	bool isDamageFlash_ = false;

	// ダメージフラッシュのタイマー
	float damageFlashTimer_ = 0.0f;

	// ダメージフラッシュの長さ
	const float kDamageFlashDuration_ = 0.4f;

	float flashAlpha_ = 1.0f;

	YKEngine::Vector4 debugColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// タイトル用自動ポーズデモ
	bool isAutoPoseDemo_ = false;

	float autoPoseTimer_ = 0.0f;

	int autoPoseIndex_ = 0;

	// 音再生用
	YKEngine::SoundData hitSE_;
	YKEngine::SoundData awaySE_;
	YKEngine::SoundData explosionSE_;
};