#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "GlobalVariables.h"
#include "imgui/imgui.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

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
	skydome_->Initialize(modelSkydome_.get(), &viewProjection_);

	//地面の生成
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get(), &viewProjection_);

	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(playerModels, &viewProjection_);
	player_->SetCameraViewProjection(&followCamera_->GetViewProjection());

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
	enemy->Initialize(enemyModels, &viewProjection_);

	enemies_.push_back(std::move(enemy));
	
	


}

void GameScene::Update() {

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
	player_->Update();

	//ロックオンの更新
	lockOn_->Update(enemies_, viewProjection_);

	//敵の更新
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Update();
	}
	
	//デバッグ表示用にワールドトランスフォームを更新
	collisionManager_->UpdateWorldTransform();

	//衝突判定と応答
	CheckAllCollisions();

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

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	//当たり判定の表示
	collisionManager_->Draw(viewProjection_);

	// スカイドームの描画
	skydome_->Draw();

	//地面の描画
	ground_->Draw();

	//自キャラの描画
	player_->Draw();

	//敵の描画
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw();
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//ロックオンの描画
	lockOn_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
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
