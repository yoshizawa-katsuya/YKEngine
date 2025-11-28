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

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

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

	viewPortMatrix_ = MakeViewportMatrix(
		0.0f,
		0.0f,
		static_cast<float>(WinApp::kClientWidth),
		static_cast<float>(WinApp::kClientHeight),
		0.0f,
		1.0f
	);

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	
	textureHandle_ = TextureManager::GetInstance()->Load("./Resources/white.png");
	textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox.dds");
	uint32_t textureHandleSceneChange = TextureManager::GetInstance()->Load("./Resources/SceneChange01_sheet.png");

	//スプライトの生成
	spriteSceneChange_ = std::make_unique<AnimatedSprite>();
	spriteSceneChange_->Initialize(textureHandleSceneChange, 20, 3);
	spriteSceneChange_->SetSize({ WinApp::kClientWidth , WinApp::kClientHeight });
	spriteSceneChange_->SetIsLoop(false);

	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateRigidModel("./Resources/player", "Player.obj");
	modelGround_ = modelPlatform_->CreateRigidModel("./Resources/ground", "Ground.obj");
	modelGround_->SetUVTransform({ {160.0f, 160.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	modelGround_->SetEnvironmentCoefficient(0.8f);

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

	//敵マネージャーの生成
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize(player_.get(), camera_.get(), &viewPortMatrix_, enemyBulletManager_.get());
	enemySpawnManager_->SetEnemyManager(enemyManager_.get());

	//デバッグカメラの生成
	camera2_ = std::make_unique<Camera>();
	camera2_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera2_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(camera2_.get(), input_);

	//メインカメラの設定
	mainCamera_ = camera_.get();

	//モデルを描画する際カメラの設定は必須
	modelPlatform_->SetCamera(mainCamera_);

	//衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();
	

	//スカイボックスの生成
	skyBox_ = std::make_unique<Rigid3dObject>();
	skyBox_->Initialize(modelPlatform_->CreateSkyBox(textureHandleSkyBox_).get());
	WorldTransform skyBoxTransform;
	skyBoxTransform.Initialize();
	skyBoxTransform.scale_ = { 100.0f, 100.0f, 100.0f };
	skyBoxTransform.UpdateMatrix();
	skyBox_->WorldTransformUpdate(skyBoxTransform);

	//地面の生成
	ground_ = std::make_unique<Rigid3dObject>();
	ground_->Initialize(modelGround_.get());
	WorldTransform groundTransform;
	groundTransform.Initialize();
	groundTransform.scale_ = { 20.0f, 20.0f, 20.0f };
	groundTransform.UpdateMatrix();
	ground_->WorldTransformUpdate(groundTransform);

}

void GameScene::Update() {

	if (isActiveDebugCamera_) 
	{
		debugCamera_->Update();
	}

	switch (phase_)
	{
	case Phase::kStart:
		UpdateStart();
		break;
	case Phase::kMain:
		UpdateMain();
		break;
	case Phase::kGameClear:
		UpdateGameClear();
		break;
	case Phase::kGameOver:
		UpdateGameOver();
		break;
	case Phase::kTitleReturn:
		UpdateTitleReturn();
		break;
	default:
		break;
	}

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	
	ParticleManager::GetInstance()->Update(mainCamera_);

#ifdef USE_IMGUI


	ImGui::Begin("Window");
	if (ImGui::TreeNode("camera")) 
	{
		ImGui::DragFloat3("translate", &camera_->GetTranslate().x, 0.01f);
		ImGui::DragFloat3("rotate", &camera_->GetRotate().x, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("DirectionalLight")) 
	{
		ImGui::ColorEdit4("color", &directionalLight_.color.x);
		ImGui::DragFloat3("direction", &directionalLight_.direction.x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_.intensity, 0.01f);

		ImGui::TreePop();
	}

	//メインカメラの切り替え
	if (ImGui::RadioButton("gameCamera", !isActiveDebugCamera_)) 
	{
		isActiveDebugCamera_ = false;

		mainCamera_ = camera_.get();
		modelPlatform_->SetCamera(mainCamera_);

	}
	if (ImGui::RadioButton("DebugCamera", isActiveDebugCamera_)) 
	{
		isActiveDebugCamera_ = true;

		mainCamera_ = camera2_.get();
		modelPlatform_->SetCamera(mainCamera_);

	}
	
	if (ImGui::Button("TitleScene")) 
	{
		phase_ = Phase::kTitleReturn;
		spriteSceneChange_->ResetReverseAnimation();
	}
	if (ImGui::Button("GameClear")) 
	{
		ProcessGameClear();
	}
	if (ImGui::Button("GameOver")) 
	{
		ProcessGameOver();
	}

	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);

	ImGui::End();
		

#endif // USE_IMGUI
	

}

void GameScene::Draw()
{

	//背景の描画
	modelPlatform_->SkyBoxPreDraw();

	skyBox_->CameraUpdate(mainCamera_);
	skyBox_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	TextureManager::GetInstance()->SetEnvironmentMap(static_cast<size_t>(ModelRootParam::kEnvironmentMap), textureHandleSkyBox_);
	
	//地面の描画
	ground_->CameraUpdate(mainCamera_);
	ground_->Draw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	//自機の弾の描画
	playerBulletManager_->Draw(mainCamera_);

	// 敵の描画
	enemyManager_->Draw(mainCamera_);

	// 弾描画
	enemyBulletManager_->Draw(mainCamera_);

	modelPlatform_->InstancingPreDraw();

	//衝突マネージャの描画
	collisionManager_->Draw(mainCamera_);

	enemySpawnManager_->Draw(mainCamera_);

	//オブジェクトの描画
	for (const auto& [name, instancingObject] : instancingObjects_) 
	{
		instancingObject->CameraUpdate(mainCamera_);
		instancingObject->Draw();
	}
	
	//パーティクルの描画
	ParticleManager::GetInstance()->Draw();

	modelPlatform_->LinePreDraw();

	railMover_->DrawRail(mainCamera_);

	//Spriteの前景描画前処理
	spritePlatform_->PreDraw();

	player_->DrawUI();

	if (phase_ != Phase::kMain) 
	{
		spriteSceneChange_->Draw();
	}

}

void GameScene::Finalize()
{

}

void GameScene::CheckAllColision() {

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
	spriteSceneChange_->Update();
	//プレイヤーの更新
	player_->Update(camera_.get());
	if (player_->StartCompleted())
	{
		phase_ = Phase::kMain;
	}
	
}

void GameScene::UpdateMain()
{

	//敵のスポーンマネージャーの更新
	enemySpawnManager_->Update();

	//イベントトリガーの更新
	eventTriggerManager_->Update();

	//レールムーバーの更新
	railMover_->Update();

	//レールカメラの更新
	railCamera_->Update();

	//プレイヤーの更新
	player_->Update(camera_.get());

	//自機の弾の更新
	playerBulletManager_->Update();

	//敵の更新
	enemyManager_->Update();

	//敵の弾の更新
	enemyBulletManager_->Update(camera_.get());

	CheckAllColision();

	player_->SetLockOnTarget(enemyManager_->GetEnemies(), camera_.get());

	CheckGameClear();

	CheckGameOver();

}

void GameScene::UpdateGameClear()
{
	spriteSceneChange_->Update();
	if (spriteSceneChange_->GetIsEnd()) 
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("ClearScene");
	}
}

void GameScene::UpdateGameOver()
{
	//プレイヤーの更新
	player_->Update(camera_.get());

	//レールカメラの更新
	railCamera_->Update();

	//敵管理クラスの更新
	enemyManager_->Update();

	//敵の弾の更新
	enemyBulletManager_->Update(camera_.get());

	CheckAllColision();

	if (!player_->GetIsGameOverEnd())
	{
		return;
	}

	spriteSceneChange_->Update();
	if (spriteSceneChange_->GetIsEnd())
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameOverScene");
	}
}

void GameScene::UpdateTitleReturn()
{
	spriteSceneChange_->Update();
	if (spriteSceneChange_->GetIsEnd())
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
}

void GameScene::CheckGameClear()
{
	if (railMover_->IsEnd()) 
	{
		//ゲームクリア
		ProcessGameClear();
	}
}

void GameScene::CheckGameOver()
{
	//プレイヤーが死んだらゲームオーバー
	if (player_->IsDead())
	{
		ProcessGameOver();
	}
}

void GameScene::ProcessGameOver()
{
	phase_ = Phase::kGameOver;
	spriteSceneChange_->ResetReverseAnimation();
	player_->GameOverRotate();
	player_->SetGameOver();
	railCamera_->SetGameOver();
	railCamera_->CreateTargetRotationFromDirection(player_->GetInverseLocalDirection());
	enemyBulletManager_->SetIsGameOver(true);
}

void GameScene::ProcessGameClear()
{
	phase_ = Phase::kGameClear;
	spriteSceneChange_->ResetReverseAnimation();
}

void GameScene::CreateLevel()
{
	LevelData levelData;
	levelData = LevelDataLoad("./resources/LevelData/", "levelData", ".json");

	assert(!levelData.splines.empty());

	//レールムーバーの生成
	railMover_ = std::make_unique<RailMover>();
	railMover_->Initialize(levelData.splines[0].controlPoints, enemySpawnManager_.get(), false);

	uint32_t heratTextureHandle = TextureManager::GetInstance()->Load("./Resources/heart.png");
	uint32_t heratFrameTextureHandle = TextureManager::GetInstance()->Load("./Resources/heartFrame.png");

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get(), &viewPortMatrix_, railMover_->GetWorldTransform(), heratTextureHandle, heratFrameTextureHandle);
	player_->SetPlayerBulletManager(playerBulletManager_.get());

	// レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	// レールカメラの初期化
	railCamera_->Initialize(camera_.get(), railMover_->GetWorldTransform(), player_->GetWorldTransform());

	for (const EnemySpawnData& enemySpawnData : levelData.enemySpawns)
	{
		EnemySpawn enemySpawn;

		//敵の種類を取得
		if (enemySpawnData.type == "EnemySpawn")
		{
			enemySpawn.type = EnemyType::kShot01;
		}
		else if (enemySpawnData.type == "TackleEnemySpawn")
		{
			enemySpawn.type = EnemyType::kTackle01;
		}
		else
		{
			assert(0 && "不明な敵の種類です");
		}
		//敵のウェーブナンバーを取得
		enemySpawn.waveNumber = enemySpawnData.waveNum.value();

		//敵の発生位置を取得
		enemySpawn.position = enemySpawnData.transform.translation;
		//敵の回転を取得
		enemySpawn.rotation = enemySpawnData.transform.rotation;

		//スプラインの制御点を取得
		if (enemySpawnData.spline.has_value())
		{
			enemySpawn.controlPoints = enemySpawnData.spline->controlPoints;
		}
		enemySpawn.speed = enemySpawnData.speed;
		
		//レベルエディターで敵のwaveNumを必ず設定するようにする
		enemySpawnManager_->AddSpawnData(enemySpawn);

	}
	
	//イベントトリガーマネージャーの生成
	eventTriggerManager_ = std::make_unique<EventTriggerManager>();

	//オブジェクトの生成
	std::string key;

	for (const ObjectData& objectData : levelData.objects)
	{

		key = objectData.fileName;

		if (key.find("Event") != std::string::npos)
		{
			//イベントトリガーの生成
			eventTriggerManager_->AddEvent(key, objectData);

			continue;
		}

		//インスタンスオブジェクトが存在しない場合は生成
		if (!instancingObjects_.contains(key))
		{
			instancingObjects_.emplace(key, std::make_unique<InstancingObjects>());
			//インスタンスオブジェクトの初期化
			if (key == "primitiveCube")
			{
				instancingObjects_[key]->Initialize(modelPlatform_->CreateCube(textureHandle_).get(), 128);
			}
			else if (key == "primitiveSphere")
			{
				instancingObjects_[key]->Initialize(modelPlatform_->CreateSphere(textureHandle_).get(), 128);
			}
		}
		//ワールド変換の初期化
		WorldTransform transform;
		transform.Initialize();
		transform.rotation_ = objectData.transform.rotation;
		transform.translation_ = objectData.transform.translation;
		transform.scale_ = objectData.transform.scale;
		transform.UpdateMatrix();
		//インスタンスオブジェクトにワールド変換を設定
		instancingObjects_[key]->WorldTransformUpdate(transform);
	}

}