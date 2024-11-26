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
#include "Boss.h"
#include "PlayerLockOn.h"
#include "Skydome.h"
#include "Ground.h"

class GameScene : public BaseScene
{
public:

	~GameScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	void CheckAllCollisions();

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

	//点光源
	std::unique_ptr<PointLight> pointLight_;

	//スポットライト
	std::unique_ptr<SpotLight> spotLight_;

	std::unique_ptr<BaseModel> modelPlayer_;
	
	std::unique_ptr<BaseModel> modelSkydome_;

	std::unique_ptr<BaseModel> modelGround_;

	uint32_t textureHandle_;
	//std::unique_ptr<Sprite> sprite_;
	
	// 天球
	std::unique_ptr<Skydome> skydome_;
	// 地面
	std::unique_ptr<Ground> ground_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	std::vector<std::unique_ptr<BaseModel>> playerModels_;

	// Boss
	std::unique_ptr<Boss> boss_;

	std::vector<std::unique_ptr<BaseModel>> bossModels_;


	std::unique_ptr<PlayerLockOn> playerLockOn_;

	// 当たり判定の大きさ
	float playerBulletRadius_ = 1.0f;

	float bossRadius_ = 10.0f;

	// ボスのHPゲージ
	std::unique_ptr<Sprite> bossHPBarSprite_;

	uint32_t bossHPBar_textureHandle_;

	Vector2 bossHPBar_position_ = {};

	// ボスの現在HP
	std::unique_ptr<Sprite> bossHPSprite_;

	uint32_t bossHP_textureHandle_;

	Vector2 bossHP_position_ = {};

	Vector2 bossHP_size_{ 1280.0f,720.0f };
};

