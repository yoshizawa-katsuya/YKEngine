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
#include "RailCamera.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Skydome.h"

class GameScene : public BaseScene
{
public:

	~GameScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllColision();

	/// <summary>
	/// 敵発生
	/// </summary>
	void EnemyPop(const Vector3& position);

	/// <summary>
	/// プレイヤーの弾を追加する
	/// </summary>
	void AddPlayerbullet(const Vector3& worldPosition, const Vector3& velocity);

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	void AddEnemybullet(const Vector3& worldPosition, const Vector3& velocity);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

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

	//3Dモデル
	std::shared_ptr<BaseModel> modelSkydome_;
	std::shared_ptr<BaseModel> modelPlayer_;

	//待機
	bool isWait_;
	int32_t waitTimer_;

	//テクスチャハンドル
	uint32_t textureHandle_;
	uint32_t enemyTextureHandle_;
	//std::unique_ptr<Sprite> sprite_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//敵
	//Enemy* enemy_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys_;

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//天球
	std::unique_ptr<Skydome> skydome_;

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_;
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