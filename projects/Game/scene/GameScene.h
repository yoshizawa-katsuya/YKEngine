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
#include "PlayerManager.h"
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

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	YKEngine::Camera* mainCamera_ = nullptr;


	std::unique_ptr<YKEngine::Camera> camera_;

	std::unique_ptr<YKEngine::DebugCamera> debugCamera_;

	bool isActiveDebugCamera_ = false;

	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	//プレイヤー管理クラス
	std::unique_ptr<PlayerManager> playerManager_;

	
};