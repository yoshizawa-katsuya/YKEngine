#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include <fstream>
#include "Matrix.h"
#include "LevelDataLoader.h"
#include "Collision.h"
#include "Vector2.h"

GameScene::~GameScene() {
	//Finalize();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	viewPortMatrix_ = MakeViewportMatrix(
		0.0f,
		0.0f,
		static_cast<float>(WinApp::kClientWidth),
		static_cast<float>(WinApp::kClientHeight),
		0.0f,
		1.0f
	);

	//平行光源の生成
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	
	//点光源の生成
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	
	//スポットライトの生成
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	
	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateRigidModel("./Resources/player", "Player.obj");
	modelPlayer_->SetEnvironmentCoefficient(1.0f);
	modelGround_ = modelPlatform_->CreateRigidModel("./Resources/ground", "Ground.obj");
	modelGround_->SetUVTransform({ {160.0f, 160.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	modelEnemy_ = modelPlatform_->CreateRigidModel("./Resources/enemy", "Enemy.obj");
	modelBullet_ = modelPlatform_->CreateSphere(textureHandle_, "Bullet");

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get(), &viewPortMatrix_);

	//エネミースポーンマネージャーの生成
	enemySpawnManager_ = std::make_unique<EnemySpawnManager>();
	enemySpawnManager_->Initialize(this);

	//ステージの生成
	CreateLevel();

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

	//textureHandle_ = TextureManager::GetInstance()->Load("./resources/circle.png");
	textureHandle_ = TextureManager::GetInstance()->Load("./Resources/white.png");
	textureHandlePlayerBullet_ = TextureManager::GetInstance()->Load("./Resources/black.png");
	textureHandleEnemyBullet_ = TextureManager::GetInstance()->Load("./Resources/red.png");
	textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox.dds");

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

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(railCamera_->GetWorldTransform());
	player_->SetGameScene(this);

	//パーティクル
	emitter_ = std::make_unique<ParticleEmitter>("HitEffect01", 3, 1.5f);
	uint32_t textureHandle3 = TextureManager::GetInstance()->Load("./Resources/circle2.png");
	emitter_->Initialize(textureHandle3, modelPlatform_->CreatePlane(textureHandle3));
	emitter_->SetScale({ 0.1f, 2.0f, 2.0f });
	emitter_->SetIsRandomScele(true);
	emitter_->SetIsRandomRotate(true);
	emitter_->SetRandRotateMax({ 0.0f, 0.0f, std::numbers::pi_v<float> });
	emitter_->SetRandRotateMin({ 0.0f, 0.0f, -std::numbers::pi_v<float> });
	emitter_->SetRandScaleMax({ 0.0f, 1.0f, 0.0f });
	emitter_->SetRandScaleMin({ 0.0f, -0.6f, 0.0f });

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);

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
	default:
		break;
	}

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_->GetDirectionalLightData());
	//modelPlatform_->PointLightUpdate(pointLight_->GetPointLightData());
	//modelPlatform_->SpotLightUpdate(spotLight_->GetSpotLightData());

	
	ParticleManager::GetInstance()->Update(mainCamera_);

	/*
	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
	*/

#ifdef _DEBUG


	ImGui::Begin("Window");
	if (ImGui::TreeNode("camera")) {
		ImGui::DragFloat3("translate", &camera_->GetTranslate().x, 0.01f);
		ImGui::DragFloat3("rotate", &camera_->GetRotate().x, 0.01f);
		//ImGui::DragFloat3("scale", &cameratransform.scale.x, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("DirectionalLight")) {
		ImGui::ColorEdit4("color", &directionalLight_->GetColor().x);
		ImGui::DragFloat3("direction", &directionalLight_->GetDirection().x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_->GetIntensity(), 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("PointLight")) {
		ImGui::ColorEdit4("color", &pointLight_->GetColor().x);
		ImGui::DragFloat3("position", &pointLight_->GetPosition().x, 0.01f);
		ImGui::DragFloat("intensity", &pointLight_->GetIntensity(), 0.01f);
		ImGui::DragFloat("radius", &pointLight_->GetRadius(), 0.01f);
		ImGui::DragFloat("decay", &pointLight_->GetDecay(), 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("SpotLight")) {
		ImGui::ColorEdit4("color", &spotLight_->GetColor().x);
		ImGui::DragFloat3("position", &spotLight_->GetPosition().x, 0.01f);
		ImGui::DragFloat("intensity", &spotLight_->GetIntensity(), 0.01f);
		ImGui::DragFloat3("direction", &spotLight_->GetDirection().x, 0.01f);
		ImGui::DragFloat("distance", &spotLight_->GetDistance(), 0.01f);
		ImGui::DragFloat("decay", &spotLight_->GetDecay(), 0.01f);
		ImGui::DragFloat("cosAngle", &spotLight_->GetCosAngle(), 0.01f);
		ImGui::DragFloat("cosFalloffStart", &spotLight_->GetCosFalloffStart(), 0.01f);

		ImGui::TreePop();
	}
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
		
	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);

	/*
	if (ImGui::Button("BGMstop")) {
		audio_->SoundStopWave(bgm1_);
	}
	*/
	ImGui::End();
		

#endif // _DEBUG
	

}

void GameScene::Draw() {

	//Spriteの背景描画前処理
	//spritePlatform_->PreBackGroundDraw();

	//sprite_->Draw();

	//背景の描画
	modelPlatform_->SkyBoxPreDraw();

	skyBox_->CameraUpdate(mainCamera_);
	skyBox_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	TextureManager::GetInstance()->SetEnvironmentMap(textureHandleSkyBox_);
	
	//modelPlatform_->SkinPreDraw();
	
	//地面の描画
	ground_->CameraUpdate(mainCamera_);
	ground_->Draw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Draw(mainCamera_);
	}

	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(mainCamera_);
	}

	// 弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(mainCamera_);
	}

	modelPlatform_->InstancingPreDraw();

	//衝突マネージャの描画
	collisionManager_->Draw(mainCamera_);

	modelPlatform_->LinePreDraw();

	railCamera_->Draw(mainCamera_);

	/*objects_->CameraUpdate(mainCamera_);
	objects_->Draw();*/
	
	//Spriteの前景描画前処理
	spritePlatform_->PreDraw();

	player_->DrawUI();

	fade_->Draw();

	ParticleManager::GetInstance()->Draw();

}

void GameScene::Finalize()
{

}

void GameScene::CheckAllColision() {

	//衝突マネージャのリセット
	collisionManager_->Reset();

	//コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		collisionManager_->AddCollider(bullet.get());
	}
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		collisionManager_->AddCollider(enemy.get());
	}
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		collisionManager_->AddCollider(bullet.get());
	}

	collisionManager_->Update();
	collisionManager_->CheckAllCollisions();

#pragma region

	Vector2 ScreenPosA = player_->Get2DReticlePosition();
	Vector2 SizeA = player_->Get2DReticleSize();

	player_->SetIsLockOn(false);

	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		Vector2 ScreenPosB = { enemy->GetScreenPosition(camera_.get()).x, enemy->GetScreenPosition(camera_.get()).y };

		if (IsCollision(Square(ScreenPosA - SizeA / 2.0f, ScreenPosA + SizeA / 2.0f), ScreenPosB)) {

			player_->LockOn(ScreenPosB, enemy->GetWorldPosition());
			break;
		}

	}

#pragma endregion
}

void GameScene::EnemyPop(const Vector3& position) {

	// 敵の生成
	std::unique_ptr<Enemy>& enemy = enemys_.emplace_back();
	// 敵の初期化
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(modelEnemy_.get(), position, &viewPortMatrix_);
	enemy->SetPlayer(player_.get());
	// 敵キャラにゲームシーンを渡す
	enemy->SetGameScene(this);

}

void GameScene::AddPlayerbullet(const Vector3& worldPosition, const Vector3& velocity)
{
	//リストに登録する
	//弾を生成し、初期化
	std::unique_ptr<PlayerBullet>& bullet = playerBullets_.emplace_back();
	bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(modelBullet_.get(), worldPosition, velocity, textureHandlePlayerBullet_);
}

void GameScene::AddEnemybullet(const Vector3& worldPosition, const Vector3& velocity) {

	//リストに登録する
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet>& bullet = enemyBullets_.emplace_back();
	bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(modelBullet_.get(), worldPosition, velocity, textureHandleEnemyBullet_);

}

void GameScene::UpdateStart()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void GameScene::UpdateMain()
{
	// フレーム時間の更新
	currentTime_ += deltaTime_;

	//敵のスポーンマネージャーの更新
	enemySpawnManager_->Update(currentTime_);

	//レールカメラの更新
	railCamera_->Update();

	//プレイヤーの更新
	player_->Update(camera_.get());

	// デスフラグの立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
		});
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Update();
	}

	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (enemy->IsDead()) {
			return true;
		}
		return false;
		});
	// 敵の更新
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
	}

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
		});
	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}

	CheckAllColision();

	CheckGameClear();

	CheckGameOver();

}

void GameScene::UpdateGameClear()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		//fade_->Stop();
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
}

void GameScene::UpdateGameOver()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		//fade_->Stop();
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
}

void GameScene::CheckGameClear()
{
	////敵がいなくなったらゲームクリア
	//if (enemys_.empty() && isEnemyPopEnd_) {
	//	phase_ = Phase::kGameClear;
	//	fade_->Start(Fade::Status::FadeOut, 0.5f);
	//}
}

void GameScene::CheckGameOver()
{
	//プレイヤーが死んだらゲームオーバー
	if (player_->IsDead()) {
		phase_ = Phase::kGameOver;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
	}
}

void GameScene::CreateLevel()
{
	LevelData* levelData;
	levelData = LevelDataLoad("./resources/LevelData/", "levelData", ".json");

	assert(!levelData->splines.empty());
	// レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	// レールカメラの初期化
	railCamera_->Initialize(levelData->splines[0].controlPoints, camera_.get());

	for (const EnemySpawnData& enemySpawnData : levelData->enemySpawns) {
		//敵の発生位置を取得
		Vector3 spawnPosition = enemySpawnData.transform.translation;

		//レベルエディターで敵のwaitTimeを必ず設定するようにする
		enemySpawnManager_->AddSpawnData(enemySpawnData.waitTime.value(), spawnPosition);

	}
	
}