#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "GlobalVariables.h"
#include "imgui/imgui.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	//グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//平行光源の生成
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize(dxCommon_);

	//点光源の生成
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize(dxCommon_);

	//スポットライトの生成
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize(dxCommon_);

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });

	//デバッグカメラの生成
	camera2_ = std::make_unique<Camera>();
	camera2_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera2_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(camera2_.get(), input_);

	//メインカメラの設定
	mainCamera_ = camera_.get();

	//モデルを描画する際ライトとカメラの設定は必須
	modelPlatform_->SetDirectionalLight(directionalLight_.get());
	modelPlatform_->SetPointLight(pointLight_.get());
	modelPlatform_->SetCamera(mainCamera_);
	modelPlatform_->SetSpotLight(spotLight_.get());

	//追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize(camera_.get());

	//衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	//ファイル名を指定してテクスチャを読み込む
	//textureHnadle_ = TextureManager::Load("./Resources/mario.jpg");

	//3Dモデルの生成
	//model_.reset(Model::Create());
	modelCollider_ = std::make_unique<RigidModel>();
	modelCollider_->CreateModel("./Resources/Collider", "Collider.obj");

	modelSphere_ = std::make_unique<RigidModel>();
	modelSphere_->CreateSphere(TextureManager::GetInstance()->Load("./Resources/white.png"));

	modelSkydome_ = std::make_unique<RigidModel>();
	modelSkydome_->CreateModel("./Resources/Skydome03", "skydome03.obj");

	modelGround_ = std::make_unique<RigidModel>();
	modelGround_->CreateModel("./Resources/Ground01", "Ground01.obj");

	//自キャラモデル
	modelFighterBody_ = std::make_unique<RigidModel>();
	modelFighterBody_->CreateModel("./Resources/float_Body", "float_Body.obj");

	modelFighterHead_ = std::make_unique<RigidModel>();
	modelFighterHead_->CreateModel("./Resources/float_Head", "float_Head.obj");

	modelFighterL_arm_ = std::make_unique<RigidModel>();
	modelFighterL_arm_->CreateModel("./Resources/float_L_arm", "float_L_arm.obj");

	modelFighterR_arm_ = std::make_unique<RigidModel>();
	modelFighterR_arm_->CreateModel("./Resources/float_R_arm", "float_R_arm.obj");

	modelHammer_ = std::make_unique<RigidModel>();
	modelHammer_->CreateModel("./Resources/Hammer01", "Hammer01.obj");

	std::vector<RigidModel*> playerModels = 
	{modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	 modelFighterR_arm_.get(), modelHammer_.get()};

	//敵モデル
	modelEnemyBody_ = std::make_unique<RigidModel>();
	modelEnemyBody_->CreateModel("./Resources/Enemy02_Body", "Enemy02_Body.obj");

	modelEnemyL_arm_ = std::make_unique<RigidModel>();
	modelEnemyL_arm_->CreateModel("./Resources/Enemy02_L_arm", "Enemy02_L_arm.obj");

	modelEnemyR_arm_ = std::make_unique<RigidModel>();
	modelEnemyR_arm_->CreateModel("./Resources/Enemy02_R_arm", "Enemy02_R_arm.obj");

	std::vector<RigidModel*> enemyModels = {modelEnemyBody_.get(), modelEnemyL_arm_.get(), modelEnemyR_arm_.get()};

	//スカイドームの生成
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	//地面の生成
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());

	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(playerModels, modelCollider_.get());

	//ロックオンの生成
	lockOn_ = std::make_unique<LockOn>();
	//ロックオンの初期化
	lockOn_->Initialize();

	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	//ロックオンをセット
	followCamera_->SetLockOn(lockOn_.get());
	player_->SetLockOn(lockOn_.get());

	//敵の生成
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	// 敵の初期化
	enemy->Initialize(enemyModels, modelCollider_.get());
	enemy->InitializeHitEffect(modelSphere_.get());

	enemies_.push_back(std::move(enemy));
	
	


}

void GameScene::Update() {

	GlobalVariables::GetInstance()->Update();

	input_->GamePadUpdate();

	// 追従カメラの更新
	followCamera_->Update();

	//カメラの更新
	camera_->Update();

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	// スカイドームの更新
	skydome_->Update();

	//地面の更新
	ground_->Update();

	//自キャラの更新
	player_->Update(mainCamera_);

	//ロックオンの更新
	lockOn_->Update(enemies_, mainCamera_);

	//敵の更新
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Update();
	}
	
	//デバッグ表示用にワールドトランスフォームを更新
	collisionManager_->UpdateWorldTransform();

	//衝突判定と応答
	CheckAllCollisions();

#ifdef _DEBUG

	ImGui::Begin("Window");
	//メインカメラの切り替え
	if (ImGui::RadioButton("gameCamera", !isActiveDebugCamera_)) {
		isActiveDebugCamera_ = false;

		mainCamera_ = camera_.get();
		modelPlatform_->SetCamera(mainCamera_);

	}
	if (ImGui::RadioButton("DebugCamera", isActiveDebugCamera_)) {
		isActiveDebugCamera_ = true;

		mainCamera_ = camera2_.get();
		modelPlatform_->SetCamera(mainCamera_);

	}
	ImGui::End();
#endif // _DEBUG

}

void GameScene::Draw() {

	
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	
	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	// 深度バッファクリア
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	modelPlatform_->PreDraw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	//当たり判定の表示
	collisionManager_->Draw(mainCamera_);

	// スカイドームの描画
	skydome_->Draw(mainCamera_);

	//地面の描画
	ground_->Draw(mainCamera_);

	//自キャラの描画
	player_->Draw(mainCamera_);

	//敵の描画
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw(mainCamera_);
	}

	// 3Dオブジェクト描画後処理
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	spritePlatform_->PreDraw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//ロックオンの描画
	lockOn_->Draw();

	// スプライト描画後処理

#pragma endregion
}

void GameScene::Finalize()
{
}

void GameScene::CheckAllCollisions() {

	//衝突マネージャのリセット
	collisionManager_->Reset();

	//コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(player_->GatHammer());
	//敵すべてについて
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		collisionManager_->AddCollider(enemy.get());
	}

	//衝突判定と応答
	collisionManager_->CheckAllCollisions();

}
