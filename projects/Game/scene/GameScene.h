
#pragma once
#include <cstdlib>
#include <memory>
#include <map>
#include "BaseScene.h"
#include "Audio.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "BaseModel.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "ParticleEmitter.h"
#include "InstancingObjects.h"
#include "RigidModel.h"
#include "DummyWall.h"
#include "LaneManager.h"
#include "Ui.h"
#include "Transition.h"
#include "CameraManager.h"
#include "Effect.h"
#include "Difficulty.h"

/// <summary>
/// ゲームシーン。
/// ゲームのメイン処理を行う。
/// </summary>
class GameScene : public YKEngine::BaseScene
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene() override;

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了。
	/// </summary>
	void Finalize() override;

private:

	/// <summary>
	/// 壁との衝突判定。
	/// </summary>
	void CheckWallCollision();

	/// <summary>
	/// レベルの生成。
	/// </summary>
	void CreateLevel();

	/// <summary>
	/// ゲームオーバー演出
	/// </summary>
	void GameOverAnimation();

	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Audio* audio_;
	YKEngine::Input* input_;

	//LoopSoundData bgm1_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	YKEngine::Camera* mainCamera_ = nullptr;


	std::unique_ptr<YKEngine::Camera> camera_;

	std::unique_ptr<YKEngine::DebugCamera> debugCamera_;

	bool isActiveDebugCamera_ = false;

	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	//点光源
	YKEngine::PointLight pointLight_;

	//スポットライト
	YKEngine::SpotLight spotLight_;

	std::shared_ptr<YKEngine::BaseModel> modelPlayer_;
	std::shared_ptr<YKEngine::BaseModel> modelWall_;

	uint32_t textureHandle_;
	uint32_t textureHandle2_;
	//std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<YKEngine::Sprite> backgroundSprite_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//ダミーのカベ
	std::unique_ptr<DummyWall> dummyWall_;

	//レーン管理
	std::unique_ptr<LaneManager> laneManager_;

	//判定
	bool isJudged_ = false;
	float prevWallZ_ = 0.0f;

	//UI
	std::unique_ptr<Ui>ui_;

	std::unique_ptr<Effect> effect_;

	//std::unique_ptr<Rigid3dObject> skyBox_;
	//WorldTransform skyBoxWorldTransform_;

	/*
	std::unique_ptr<InstancingObjects> objects_;

	WorldTransform worldTransform1_;
	WorldTransform worldTransform2_;
	*/

	//パーティクル
	//std::unique_ptr<ParticleEmitter> emitter_;
	//std::unique_ptr<AccelerationField> field_;
	//Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	// 難易度
	Difficulty difficulty_ = Difficulty::EASY;

	// 遷移演出
	std::unique_ptr<Transition> transition_;

	// 次のシーン名
	std::string nextSceneName_;

	//デバッグ用
	YKEngine::Vector4 debugPlayerColor[2] = { {1.0f,0.0f,1.0f,1.0f},{0.0f,1.0f,1.0f,1.0f} };
	int32_t debugScore_ = 0;
	int32_t debugMiss_ = 0;
	int32_t debugCombo_ = 0;
	int32_t debugMaxCombo_ = 0;
	
	// ゲームオーバー演出用
	// 通常時FOV
	float normalFov_;

	// 演出用FOV
	float gameOverFov_ = 0.5f;

	// 現在の補間速度
	float fovLerpSpeed_ = 0.4f;

	// 横揺れ
	float cameraShakeTimer_ = 0.0f;
	// 横揺れの強さ
	float shakeStrength_ = 0.3f;

	float damageShakeTimer_ = 0.0f;

	// シェイク前の基準位置
	YKEngine::Vector3 damageShakeBasePos_ = {};

	// 爆発シェイク
	float explosionShakeTimer_ = 0.0f;
	float explosionShakeDuration_ = 2.0f;

	Vector3 explosionShakeBasePos_ = {};

	bool isExplosionShakeStarted_ = false;
	bool isExplosionShakeFinished_ = false;

	// 演出用カメラ
	YKEngine::Vector3 gameOverCameraPos_ = { 0.0f, 3.6f, -10.0f };
	YKEngine::Vector3 gameOverCameraTarget_ = { 0.0f, 0.0f, 0.0f };

	std::unique_ptr<CameraManager> cameraManager_ = nullptr;

	enum CameraMode {
		Free,
		GameOver,
	};

	CameraMode cameraMode_ = CameraMode::Free;
	//遷移開始済みか
	bool isStartedTransition_ = false;

	float deathFinishedTimer_ = 0.0f;
	bool isDeathFinishedTimerStarted_ = false;
};