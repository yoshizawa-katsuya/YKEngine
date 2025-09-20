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
#include "PlayerBullet.h"
#include "CollisionManager.h"
#include "Fade.h"
#include "manager/EnemySpawnManager.h"
#include "RailMover.h"
#include "BaseEventTrigger.h"

class GameScene : public BaseScene
{
public:

	~GameScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	/// <summary>
	/// プレイヤーの弾を追加する
	/// </summary>
	void AddPlayerbullet(const Vector3& worldPosition, const Vector3& velocity);

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	void AddEnemybullet(const Vector3& worldPosition, const Vector3& velocity);

	/// <summary>
	/// 敵発生
	/// </summary>
	void EnemyPop(const Vector3& position, const Vector3& rotation, const std::vector<Vector3>& controlPoints);

private:

	void UpdateStart();

	void UpdateMain();

	void UpdateGameClear();

	void UpdateGameOver();

	//クリアしたかどうかを判定する
	void CheckGameClear();

	//ゲームオーバーかどうかを判定する
	void CheckGameOver();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllColision();

	/// <summary>
	/// レベルの生成
	/// </summary>
	void CreateLevel();

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	//LoopSoundData bgm1_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	Camera* mainCamera_ = nullptr;

	Matrix4x4 viewPortMatrix_;

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
	/*std::shared_ptr<BaseModel> modelSkydome_;*/
	std::shared_ptr<BaseModel> modelGround_;
	std::shared_ptr<BaseModel> modelPlayer_;
	std::shared_ptr<BaseModel> modelEnemy_;
	std::shared_ptr<BaseModel> modelBullet_;

	//テクスチャハンドル
	uint32_t textureHandle_;
	uint32_t textureHandlePlayerBullet_;
	uint32_t textureHandleEnemyBullet_;
	uint32_t textureHandleSkyBox_;
	//std::unique_ptr<Sprite> sprite_;

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_;

	std::unique_ptr<EnemySpawnManager> enemySpawnManager_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;

	//敵
	//Enemy* enemy_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys_;

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//天球
	/*std::unique_ptr<Skydome> skydome_;*/

	//スカイボックス
	std::unique_ptr<Rigid3dObject> skyBox_;

	//地面
	std::unique_ptr<Rigid3dObject> ground_;

	//オブジェクト
	std::map<std::string, std::unique_ptr<InstancingObjects>> instancingObjects_;

	//レールムーバー
	std::unique_ptr<RailMover> railMover_;

	//イベント
	std::list<std::unique_ptr<BaseEventTrigger>> events_;

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_;
	/*
	std::unique_ptr<InstancingObjects> objects_;

	WorldTransform worldTransform1_;
	WorldTransform worldTransform2_;
	*/

	//パーティクル
	std::unique_ptr<ParticleEmitter> emitter_;
	//std::unique_ptr<AccelerationField> field_;
	//Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	//シーンのフェーズ
	enum class Phase {
		kStart,	//開始部
		kMain,	//メイン部
		kGameClear,	//クリア部
		kGameOver,	//ゲームオーバー部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kStart;

	std::unique_ptr<Fade> fade_;

};