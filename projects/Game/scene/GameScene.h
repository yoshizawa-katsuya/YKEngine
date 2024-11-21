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
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "ParticleEmitter.h"
#include "Boss.h"
class GameScene : public BaseScene
{
public:

	~GameScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	//LoopSoundData bgm1_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	Camera* mainCamera_ = nullptr;

	
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Camera> camera2_;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isActiveDebugCamera_ = false;

	//平行光源
	std::unique_ptr<DirectionalLight> directionalLight_;

	std::unique_ptr<BaseModel> modelPlayer_;
	

	uint32_t textureHandle_;
	//std::unique_ptr<Sprite> sprite_;

	//プレイヤー
	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<BaseModel>> playerModels_;


	//Boss
	std::vector<std::unique_ptr<BaseModel>> bossModels_;
	std::unique_ptr<Boss> boss_;

};

