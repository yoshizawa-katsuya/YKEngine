#pragma once
#include <cstdlib>
#include <memory>
#include <map>
#include "BaseScene.h"
#include "Struct.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "Sprite.h"
#include "Player.h"
#include "Object3d.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "ParticleEmitter.h"
#include "Animation.h"

class GameScene : public BaseScene
{
public:

	~GameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	int blendMode = static_cast<int>(BlendMode::kBlendModeNone);

	Camera* mainCamera_ = nullptr;

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Camera> camera2_;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isActiveDebugCamera_ = false;

	//SoundData soundData1_;

	//平行光源
	std::unique_ptr<DirectionalLight> directionalLight_;

	std::unique_ptr<Model> model_;

	//プレイヤー
	std::unique_ptr<Player> player_;

};

