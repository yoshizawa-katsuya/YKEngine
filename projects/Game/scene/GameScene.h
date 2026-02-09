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


	uint32_t textureHandle_;
	uint32_t textureHandle2_;
	std::unique_ptr<YKEngine::Sprite> sprite_;

	//プレイヤー
	std::unique_ptr<Player> player_;

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
	
};