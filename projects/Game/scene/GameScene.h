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
#include "AnimatedSprite.h"
#include "Player.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "InstancingObjects.h"
#include "RigidModel.h"
#include "RailCamera.h"
#include "manager/EnemyManager.h"
#include "Skydome.h"
#include "manager/CollisionManager.h"
#include "manager/EnemySpawnManager.h"
#include "RailMover.h"
#include "manager/EventTriggerManager.h"
#include "manager/PlayerBulletManager.h"
#include "manager/EnemyBulletManager.h"

/// <summary>
/// ゲームシーン。
/// ゲームのメイン処理を行う。
/// </summary>
class GameScene : public BaseScene
{
public:

	~GameScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	void UpdateStart();

	void UpdateMain();

	void UpdateGameClear();

	void UpdateGameOver();

	void UpdateTitleReturn();

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
	std::shared_ptr<BaseModel> modelGround_;
	std::shared_ptr<BaseModel> modelPlayer_;

	//テクスチャハンドル
	uint32_t textureHandle_;
	uint32_t textureHandleSkyBox_;

	//スプライト
	std::unique_ptr<AnimatedSprite> spriteSceneChange_;//シーンチェンジのスプライト

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_;

	std::unique_ptr<EnemySpawnManager> enemySpawnManager_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//プレイヤーの弾
	std::unique_ptr<PlayerBulletManager> playerBulletManager_;

	//敵
	std::unique_ptr<EnemyManager> enemyManager_;

	// 敵の弾
	std::unique_ptr<EnemyBulletManager> enemyBulletManager_;

	//スカイボックス
	std::unique_ptr<Rigid3dObject> skyBox_;

	//地面
	std::unique_ptr<Rigid3dObject> ground_;

	//オブジェクト
	std::map<std::string, std::unique_ptr<InstancingObjects>> instancingObjects_;

	//レールムーバー
	std::unique_ptr<RailMover> railMover_;

	//イベントトリガーマネージャー
	std::unique_ptr<EventTriggerManager> eventTriggerManager_;

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_;

	//シーンのフェーズ
	enum class Phase {
		kStart,	//開始部
		kMain,	//メイン部
		kGameClear,	//クリア部
		kGameOver,	//ゲームオーバー部
		kTitleReturn, //タイトルへ戻る
	};

	//現在のフェーズ
	Phase phase_ = Phase::kStart;

};