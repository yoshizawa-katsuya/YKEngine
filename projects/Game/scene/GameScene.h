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

	/// <summary>
	/// 開始部の更新。
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// メイン部の更新。
	/// </summary>
	void UpdateMain();

	/// <summary>
	/// クリア部の更新。
	/// </summary>
	void UpdateGameClear();

	/// <summary>
	/// ゲームオーバー部の更新。
	/// </summary>
	void UpdateGameOver();

	/// <summary>
	/// タイトルへ戻る部の更新。
	/// </summary>
	void UpdateTitleReturn();

	/// <summary>
	/// ゲームクリアかどうかを判定する。
	/// </summary>
	void CheckGameClear();

	/// <summary>
	/// ゲームオーバーかどうかを判定する。
	/// </summary>
	void CheckGameOver();

	/// <summary>
	/// ゲームオーバーに移行する際の処理。
	/// </summary>
	void ProcessGameOver();

	/// <summary>
	/// クリアに移行する際の処理。
	/// </summary>
	void ProcessGameClear();

	/// <summary>
	/// 全ての衝突判定を行う。
	/// </summary>
	void CheckAllColision();

	/// <summary>
	/// レベルを作成する。
	/// </summary>
	void CreateLevel();

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

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