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
#include "BaseEnemy.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "ParticleEmitter.h"
#include "InstancingObjects.h"
#include "CameraController.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"

class GameScene : public BaseScene
{
public:

	~GameScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	void AddPlayerBullet(const Vector3& velocity);

	void AddEnemyBullet(const Vector3& position, const Vector3& velocity);

private:

	//敵の配置
	void CreateLevel();

	// 衝突判定と応答
	void CheckAllCollisions();

	//自キャラの攻撃の衝突判定
	void CheackPlayerAttackCollision();

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

	std::unique_ptr<CameraController> cameraController_;

	bool isActiveDebugCamera_ = false;

	//平行光源
	std::unique_ptr<DirectionalLight> directionalLight_;

	//点光源
	std::unique_ptr<PointLight> pointLight_;

	//スポットライト
	std::unique_ptr<SpotLight> spotLight_;

	std::unique_ptr<BaseModel> modelPlayer_;
	std::unique_ptr<BaseModel> modelHammer_;
	std::unique_ptr<BaseModel> modelEnemy01_;
	std::unique_ptr<BaseModel> modelEnemy02_;
	std::unique_ptr<BaseModel> modelEnemy03_;
	std::unique_ptr<BaseModel> modelGround_;
	std::unique_ptr<BaseModel> modelHPGauge_;
	std::unique_ptr<BaseModel> modelPlayerBullet_;
	std::unique_ptr<BaseModel> modelEnemyBullet_;

	uint32_t textureHandle_;
	uint32_t textureHandleHeartFrame_;
	uint32_t textureHandleHeart_;
	uint32_t textureHandleBlue_;
	uint32_t textureHandleDarkRed_;
	uint32_t textureHandleRed_;
	//std::unique_ptr<Sprite> sprite_;

	//プレイヤー
	std::unique_ptr<Player> player_;
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;
	const uint32_t kMaxPlayerBulletsNum_ = 3;

	std::unique_ptr<Base3dObject> ground_;

	//敵
	//std::unique_ptr<BaseEnemy> enemy_;
	std::list<std::unique_ptr<BaseEnemy>> enemies_;
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//パーティクル
	/*
	std::unique_ptr<ParticleEmitter> emitter_;
	std::unique_ptr<AccelerationField> field_;
	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	*/
};