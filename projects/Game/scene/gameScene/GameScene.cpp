#include "GameScene.h"
#include "dx12.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include <fstream>
#include "Matrix.h"
#include "LevelDataLoader.h"
#include "Vector2.h"
#include "ShotEnemy01.h"
#include "RootParams.h"
#include "SceneChangeStaging.h"
#include "GameSceneStartState.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GameScene::~GameScene() 
{
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	dxCommon_->ResetDeltaTime();

	//スプライトの生成
	operationGuideSprite_ = std::make_unique<Sprite>();
	operationGuideSprite_->Initialize(TextureManager::GetInstance()->Load("./Resources/operationHUD.png"));

	//ステージオブジェクトの生成
	stageObjects_ = std::make_unique<StageObjects>();
	stageObjects_->Initialize();

	//自機の弾マネージャーの生成
	playerBulletManager_ = std::make_unique<PlayerBulletManager>();
	playerBulletManager_->Initialize();

	//敵の弾マネージャーの生成
	enemyBulletManager_ = std::make_unique<EnemyBulletManager>();
	enemyBulletManager_->Initialize();

	//エネミースポーンマネージャーの生成
	enemySpawnManager_ = std::make_unique<EnemySpawnManager>();
	enemySpawnManager_->Initialize();

	//ステージの生成
	CreateLevel();

	//カメラの生成
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->InitializeBeforeGameStart(railMover_->GetWorldTransform(), player_->GetWorldTransform());
	player_->SetRailCamera(cameraManager_->GetRailCameraInner());

	//敵マネージャーの生成
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize(player_.get(), cameraManager_->GetRailCameraInner(), enemyBulletManager_.get());
	enemySpawnManager_->SetEnemyManager(enemyManager_.get());

	//衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	//シーン開始演出の開始
	sceneChangeStaging_ = SceneChangeStaging::GetInstance();
	sceneChangeStaging_->BeginSceneStart(StagingType::kFade);

	//ポーズ画面の生成
	pause_ = std::make_unique<Pause>();
	pause_->Initialize();

	//ステートマシンの生成と開始
	stateMachine_ = std::make_unique<StateMachine<GameSceneStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<GameSceneStartState>();
}

void GameScene::Update()
{
	
	//カメラマネージャーの更新
	cameraManager_->Update();

	//ステージオブジェクトの更新
	stageObjects_->Update();

	//ステートマシンの更新
	stateMachine_->Update();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	
#ifdef USE_IMGUI

	ImGui::Begin("GameScene");

	if (ImGui::TreeNode("DirectionalLight"))
	{
		ImGui::ColorEdit4("color", &directionalLight_.color.x);
		ImGui::DragFloat3("direction", &directionalLight_.direction.x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_.intensity, 0.01f);

		ImGui::TreePop();
	}
	
	if (ImGui::Button("TitleScene")) 
	{
		isReturnToTitleScene_ = true;
	}
	if (ImGui::Button("GameClear")) 
	{
		isClear_ = true;
	}
	if (ImGui::Button("GameOver")) 
	{
		player_->SetIsDead(true);
	}

	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);

	ImGui::End();
		

#endif // USE_IMGUI
	

}

void GameScene::Draw()
{
	//メインカメラを取得
	Camera* mainCamera = cameraManager_->GetMainCamera();

	//背景の描画
	modelPlatform_->SkyBoxPreDraw();

	stageObjects_->DrawSkyBox(mainCamera);

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	TextureManager::GetInstance()->SetEnvironmentMap(static_cast<size_t>(ModelRootParam::kEnvironmentMap), stageObjects_->GetTextureHandleSkyBox());

	stageObjects_->Draw(mainCamera);

	//プレイヤーの描画
	player_->Draw(mainCamera);

	//自機の弾の描画
	playerBulletManager_->Draw(mainCamera);

	// 敵の描画
	enemyManager_->Draw(mainCamera);

	// 弾描画
	enemyBulletManager_->Draw(mainCamera);

	modelPlatform_->InstancingPreDraw();

	//衝突マネージャの描画
	collisionManager_->Draw(mainCamera);

	enemySpawnManager_->Draw(mainCamera);

	//ステージオブジェクトの描画
	stageObjects_->InstancingDraw(mainCamera);
	
	//パーティクルの描画
	ParticleManager::GetInstance()->Draw();

	modelPlatform_->LinePreDraw();

	railMover_->DrawRail(mainCamera);

	//Spriteの前景描画前処理
	spritePlatform_->PreDraw();

	player_->DrawUI();

	//操作説明HUDの描画
	operationGuideSprite_->Draw();

	//ポーズ画面の描画
	pause_->Draw();

	sceneChangeStaging_->Draw();
}

void GameScene::Finalize()
{

}

void GameScene::CheckAllColision()
{

	//衝突マネージャのリセット
	collisionManager_->Reset();

	//コライダーをリストに登録
	collisionManager_->AddCollider(railMover_.get());
	collisionManager_->AddCollider(player_.get());
	playerBulletManager_->RegisterToCollisionManager(collisionManager_.get());
	enemyManager_->RegisterToCollisionManager(collisionManager_.get());
	enemyBulletManager_->RegisterToCollisionManager(collisionManager_.get());
	eventTriggerManager_->RegisterToCollisionManager(collisionManager_.get());
	

	collisionManager_->Update();
	collisionManager_->CheckAllCollisions();

}

void GameScene::UpdateStart()
{
	ParticleManager::GetInstance()->Update(cameraManager_->GetMainCamera());

	//レールカメラの更新
	cameraManager_->UpdateRailCamera();

	//プレイヤーの更新
	player_->Update();
	
}

void GameScene::UpdateMain()
{
	ParticleManager::GetInstance()->Update(cameraManager_->GetMainCamera());

	//レールカメラの取得
	Camera* railCamera = cameraManager_->GetRailCameraInner();

	//敵のスポーンマネージャーの更新
	enemySpawnManager_->Update();

	//イベントトリガーの更新
	eventTriggerManager_->Update();

	//レールムーバーの更新
	railMover_->Update();

	//レールカメラの更新
	cameraManager_->UpdateRailCamera();

	//プレイヤーの更新
	player_->Update();

	//自機の弾の更新
	playerBulletManager_->Update();

	//敵の更新
	enemyManager_->Update();

	//敵の弾の更新
	enemyBulletManager_->Update(railCamera);

	CheckAllColision();

	player_->SetLockOnTarget(enemyManager_->GetEnemies());

	CheckGameClear();

	CheckGameOver();

}

void GameScene::UpdateGameClear()
{
	ParticleManager::GetInstance()->Update(cameraManager_->GetMainCamera());

	player_->Update();

	if (sceneChangeStaging_->IsFinished()) 
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("ClearScene");
	}
}

void GameScene::UpdateGameOver()
{
	ParticleManager::GetInstance()->Update(cameraManager_->GetMainCamera());

	//レールカメラの取得
	Camera* railCamera = cameraManager_->GetRailCameraInner();

	//プレイヤーの更新
	player_->Update();

	//自機の弾の更新
	playerBulletManager_->Update();

	//レールカメラの更新
	cameraManager_->UpdateRailCamera();

	//敵管理クラスの更新
	enemyManager_->Update();

	//敵の弾の更新
	enemyBulletManager_->Update(railCamera);

	CheckAllColision();

	if (!player_->GetIsGameOverEnd())
	{
		return;
	}

	//シーン終了演出開始
	if (isGameOverSceneChangeStagingStart_)
	{
		//黒でフェードアウト
		StartSceneEndStaging({0.0f, 0.0f, 0.0f, 1.0f});
		isGameOverSceneChangeStagingStart_ = false;
	}

	if (sceneChangeStaging_->IsFinished())
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameOverScene");
	}
}

void GameScene::UpdateTitleReturn()
{

	if (sceneChangeStaging_->IsFinished())
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
}

void GameScene::UpdatePause()
{
	pause_->Update();
}

void GameScene::ProcessGameOver()
{
	isGameOverSceneChangeStagingStart_ = true;
	player_->SetIsDead(true);
	cameraManager_->ProcessGameOver(player_->GetInverseLocalDirection());
	enemyBulletManager_->SetIsGameOver(true);
}

void GameScene::ProcessGameClear()
{
	//白でフェードアウト
	StartSceneEndStaging({1.0f, 1.0f, 1.0f, 1.0f});
	player_->SetGameClear();
}

void GameScene::ProcessPause()
{
	pause_->SetIsPause(true);
}

void GameScene::StartSceneEndStaging(const YKEngine::Vector4& color)
{
	sceneChangeStaging_->BeginSceneEnd(StagingType::kFade, color);
}

void GameScene::CreateLevel()
{
	LevelData levelData;
	levelData = LevelDataLoad("./resources/LevelData/", "levelData", ".json");

	assert(!levelData.splines.empty());

	//レールムーバーの生成
	railMover_ = std::make_unique<RailMover>();
	railMover_->Initialize(levelData.splines[0].controlPoints, enemySpawnManager_.get(), false);

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(railMover_->GetWorldTransform());
	player_->SetPlayerBulletManager(playerBulletManager_.get());

	//エネミースポーンデータの設定
	enemySpawnManager_->GetSpawnDatas(levelData.enemySpawns);
	
	//イベントトリガーマネージャーの生成
	eventTriggerManager_ = std::make_unique<EventTriggerManager>();

	//イベントトリガーの生成
	eventTriggerManager_->CreateEventTriggers(levelData.objects);

	//イベントオブジェクトのデータをレベルデータから削除
	std::erase_if(levelData.objects, [](const ObjectData& data) {return data.fileName.find("Event") != std::string::npos; });

	// 背景オブジェクトの生成
	stageObjects_->GetInstancingObject(levelData.objects);

}