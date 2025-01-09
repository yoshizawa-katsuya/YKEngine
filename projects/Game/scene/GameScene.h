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
#include "MapChipField.h"

class GameScene : public BaseScene
{
public:

	~GameScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	//マップの生成
	void GeneratrBlocks();



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

	//点光源
	std::unique_ptr<PointLight> pointLight_;

	//スポットライト
	std::unique_ptr<SpotLight> spotLight_;

	std::unique_ptr<BaseModel> modelPlayer_;


	uint32_t textureHandle_;
	//std::unique_ptr<Sprite> sprite_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//
	std::unique_ptr<Sprite> selectEasySprite_ = nullptr;
	uint32_t easySprite_ = 0;
	std::unique_ptr<Sprite> selectNormalSprite_ = nullptr;
	uint32_t normalSprite_ = 0;
	std::unique_ptr<Sprite> selectHardSprite_ = nullptr;
	uint32_t hardSprite_ = 0;

	//map chip
	std::unique_ptr<MapChipField> mapChipField_;

	//ステージの壁
	std::vector<std::vector<std::unique_ptr<WorldTransform>>> worldTransformBlocks_;

	std::unique_ptr<BaseModel> modelBlock_;


	//パーティクル
	/*
	std::unique_ptr<ParticleEmitter> emitter_;
	std::unique_ptr<AccelerationField> field_;
	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	*/
};