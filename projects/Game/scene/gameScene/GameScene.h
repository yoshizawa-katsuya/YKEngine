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
#include "InstancingObjects.h"
#include "RigidModel.h"
#include "manager/EnemyManager.h"
#include "manager/CollisionManager.h"
#include "manager/EnemySpawnManager.h"
#include "RailMover.h"
#include "manager/EventTriggerManager.h"
#include "manager/PlayerBulletManager.h"
#include "manager/EnemyBulletManager.h"
#include "manager/CameraManager.h"
#include "StageObjects.h"
#include "StateMachine.hpp"
#include "GameSceneStateContext.h"
#include "Pause.h"
#include "OperationGuide.h"
class SceneChangeStaging;

/// <summary>
/// ゲームシーン。
/// ゲームのメイン処理を行う。 
/// </summary>
class GameScene : public YKEngine::BaseScene, private GameSceneStateContext
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
	void UpdateStart() override;

	/// <summary>
	/// メイン部の更新。
	/// </summary>
	void UpdateMain() override;

	/// <summary>
	/// クリア部の更新。
	/// </summary>
	void UpdateGameClear() override;

	/// <summary>
	/// ゲームオーバー部の更新。
	/// </summary>
	void UpdateGameOver() override;

	/// <summary>
	/// タイトルへ戻る部の更新。
	/// </summary>
	void UpdateTitleReturn() override;

	/// <summary>
	/// ポーズ画面の更新。
	/// </summary>
	void UpdatePause() override;

	/// <summary>
	/// ポーズ画面を取得する。
	/// </summary>
	/// <returns> ポーズ画面 </returns>
	Pause* GetPause() override { return pause_.get(); }

	bool StartCompleted() override { return player_->StartCompleted(); }

	/// <summary>
	/// ゲームクリアかどうかを判定する。
	/// </summary>
	bool CheckGameClear() override { return railMover_->IsEnd() || isClear_; }

	/// <summary>
	/// ゲームオーバーかどうかを判定する。
	/// </summary>
	bool CheckGameOver() override { return player_->IsDead(); }

	/// <summary>
	/// タイトルへ戻るときにtrueを返す。
	/// </summary>
	bool IsReturnToTitle() override { return isReturnToTitleScene_; }

	/// <summary>
	/// ゲームオーバーに移行する際の処理。
	/// </summary>
	void ProcessGameOver() override;

	/// <summary>
	/// クリアに移行する際の処理。
	/// </summary>
	void ProcessGameClear() override;

	/// <summary>
	/// ポーズ画面に移行する際の処理。
	/// </summary>
	void ProcessPause() override;

	/// <summary>
	/// スタートからメインに移行する際の処理。
	/// </summary>
	void ExitStart() override;

	/// <summary>
	/// シーン終了演出の開始。
	/// </summary>
	void StartSceneEndStaging(const YKEngine::Vector4& color) override;

	/// <summary>
	/// レベルを作成する。
	/// </summary>
	void CreateLevel();

	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Audio* audio_;
	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	std::unique_ptr<CameraManager> cameraManager_;

	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	std::unique_ptr<OperationGuide> operationGuide_;

	//シーンチェンジ演出
	SceneChangeStaging* sceneChangeStaging_ = nullptr;
	bool isGameOverSceneChangeStagingStart_ = false;

	//衝突マネージャー
	CollisionManager* collisionManager_;

	std::unique_ptr<EnemySpawnManager> enemySpawnManager_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//プレイヤーの弾
	std::unique_ptr<PlayerBulletManager> playerBulletManager_;

	//敵
	std::unique_ptr<EnemyManager> enemyManager_;

	// 敵の弾
	std::unique_ptr<EnemyBulletManager> enemyBulletManager_;

	//ステージオブジェクト
	std::unique_ptr<StageObjects> stageObjects_;

	//レールムーバー
	std::unique_ptr<RailMover> railMover_;

	//イベントトリガーマネージャー
	std::unique_ptr<EventTriggerManager> eventTriggerManager_;

	//ステートマシン
	std::unique_ptr<YKEngine::StateMachine<GameSceneStateContext>> stateMachine_;

	//タイトルへ戻るときはtrue
	bool isReturnToTitleScene_ = false;

	//クリアしていたらtrue。デバッグ用。
	bool isClear_ = false;

	//ポーズ画面
	std::unique_ptr<Pause> pause_;

};