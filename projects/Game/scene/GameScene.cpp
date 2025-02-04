#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "RigidModel.h"
#include "Rigid3dObject.h"
#include "LevelDataLoader.h"
#include "Collision.h"
#include "Enemy01.h"
#include "Enemy02.h"
#include "Enemy03.h"

uint32_t GameScene::stageNum_ = 1;

GameScene::~GameScene() {
	//Finalize();
	audio_->SoundStopWave(bgm1_);
	//audio_->SoundUnload(&bgm1_);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	threadPool_ = ThreadPool::GetInstance();
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

	textureHandle_ = TextureManager::GetInstance()->Load("./resources/circle.png");

	textureHandleHeartFrame_ = TextureManager::GetInstance()->Load("./resources/heartFrame.png");
	textureHandleHeart_ = TextureManager::GetInstance()->Load("./resources/heart.png");
	textureHandleBlue_ = TextureManager::GetInstance()->Load("./resources/blue1x1.png");
	textureHandleDarkRed_ = TextureManager::GetInstance()->Load("./resources/darkRed1x1.png");
	textureHandleRed_ = TextureManager::GetInstance()->Load("./resources/red1x1.png");

	//モデルの生成
	modelPlayer_ = std::make_unique<RigidModel>();
	modelPlayer_->CreateModel("./resources/Player", "Player.obj");
	//modelPlayer_->CreateSphere(textureHandleTitle_);
	
	modelHammer_ = std::make_unique<RigidModel>();
	modelHammer_->CreateModel("./resources/Hammer", "Hammer.obj");

	modelEnemy01_ = std::make_unique<RigidModel>();
	modelEnemy01_->CreateModel("./resources/enemy", "Enemy.obj");

	modelEnemy02_ = std::make_unique<RigidModel>();
	modelEnemy02_->CreateModel("./resources/enemy02", "Enemy02.obj");

	modelEnemy03_ = std::make_unique<RigidModel>();
	modelEnemy03_->CreateModel("./resources/enemy03", "Enemy03.obj");

	modelGround_ = std::make_unique<RigidModel>();
	modelGround_->CreateModel("./resources/ground", "Ground.obj");
	modelGround_->SetUVTransform({ 25.0f, 25.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

	modelSkyDome_ = std::make_unique<RigidModel>();
	modelSkyDome_->CreateModel("./resources/skyDome", "skydome.obj");
	modelSkyDome_->SetEnableLighting(false);

	modelHPGauge_ = std::make_unique<RigidModel>();
	modelHPGauge_->CreateModel("./resources/HPGauge", "HPGauge.obj");
	modelHPGauge_->SetEnableLighting(false);

	modelHPGaugeFrame_ = std::make_unique<RigidModel>();
	modelHPGaugeFrame_->CreateModel("./resources/HPGaugeFrame", "HPGaugeFrame.obj");
	modelHPGaugeFrame_->SetEnableLighting(false);

	modelPlayerBullet_ = std::make_unique<RigidModel>();
	modelPlayerBullet_->CreateSphere(textureHandleBlue_);
	//modelPlayerBullet_->SetEnableLighting(false);

	modelEnemyBullet_ = std::make_unique<RigidModel>();
	modelEnemyBullet_->CreateSphere(textureHandleRed_);

	ground_ = std::make_unique<Rigid3dObject>();
	ground_->Initialize(modelGround_.get());

	skydome_ = std::make_unique<Rigid3dObject>();
	skydome_->Initialize(modelSkyDome_.get());

	/*
	//テクスチャハンドルの生成
	textureHandleTitle_ = TextureManager::GetInstance()->Load("./resources/player/Player.png");

	//スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandleTitle_, spritePlatform_);
	*/

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());
	player_->HammerInitialize(modelHammer_.get());
	player_->HUDInitialize(textureHandleHeartFrame_, textureHandleHeart_);
	player_->SetGameScene(this);

	cameraController_ = std::make_unique<CameraController>();
	cameraController_->Initialize(camera_.get(), player_.get());

	//敵の生成
	//enemy_ = std::make_unique<BaseEnemy>();
	//enemy_->Initialize(modelEnemy01_.get());
	CreateLevel();

	bgm1_ = audio_->LoopSoundLoadWave("./resources/sound/gameScene.wav");
	audio_->SoundLoopPlayWave(bgm1_, 0.5f);

	HitSE1_ = audio_->SoundLoadWave("./resources/sound/HitSE1.wav");
	HitSE2_ = audio_->SoundLoadWave("./resources/sound/HitSE2.wav");
	HitSE3_ = audio_->SoundLoadWave("./resources/sound/HitSE3.wav");
	JumpSE1_ = audio_->SoundLoadWave("./resources/sound/JumpSE1.wav");
	LandingSE1_ = audio_->SoundLoadWave("./resources/sound/LandingSE1.wav");
	SwingSE1_ = audio_->SoundLoadWave("./resources/sound/SwingSE1.wav");
	ShotSE1_ = audio_->SoundLoadWave("./resources/sound/ShotSE1.wav");

	player_->SetHitSEData(&HitSE1_);
	player_->SetHitSE2Data(&HitSE2_);
	player_->SetJumpSE(&JumpSE1_);
	player_->SetLandingSE(&LandingSE1_);
	player_->SetSwingSE(&SwingSE1_);

	emitter_ = std::make_unique<ParticleEmitter>("bord", 5, 0.5f);
	emitter_->Initialize(textureHandle_);
	emitter_->SetRandTranslationMin({ 0.0f, 0.0f, 0.0f });
	emitter_->SetRandTranslationMax({ 0.0f, 0.0f, 0.0f });
	emitter_->SetScale({ 0.3f, 0.3f, 1.0f });
	//emitter_->SetIsRandomColor(true);

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);
}

void GameScene::Update() {


	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	float playerPositionX = 0.0f;

	switch (phase_) {
	case GameScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kMain;
		}
		break;

	case GameScene::Phase::kMain:
		//プレイヤーの更新
		player_->Update();

		//デスフラグの立ったオブジェクトを削除
		CheckAllDelete();

		playerPositionX = player_->GetCenterPosition().x;
		//自キャラの弾の更新
		for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets_) {
			playerBullet->Update(playerPositionX);
		}

		//敵の更新
		//enemy_->Update();
		for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
			enemy->Update();
		}
		threadPool_->waitForCompletion();

		//敵の弾の更新
		for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_) {
			enemyBullet->Update();
		}
		
		CheckAllCollisions();

		if (enemies_.size() == 0) {
			fade_->Start(Fade::Status::FadeOut, 0.5f);
			phase_ = Phase::kClear;
		}
		else if (!player_->GetIsAlive()) {
			fade_->Start(Fade::Status::FadeOut, 0.5f);
			phase_ = Phase::kGameOver;
		}
		break;

	case GameScene::Phase::kClear:
		fade_->Update();
		if (fade_->IsFinished()) {
			//fade_->Stop();
			//シーン切り替え依頼
			sceneManager_->ChengeScene("ClearScene");
		}
		break;
	
	case GameScene::Phase::kGameOver:
		fade_->Update();
		if (fade_->IsFinished()) {
			//fade_->Stop();
			//シーン切り替え依頼
			sceneManager_->ChengeScene("GameOverScene");
		}
		break;
	}
	

	//カメラコントローラーの更新
	cameraController_->Update();

	//カメラの更新
	camera_->Update();

	//emitter_->Update();

	ParticleManager::GetInstance()->Update(mainCamera_);

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

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//modelPlatform_->SkinPreDraw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets_) {
		playerBullet->Draw(mainCamera_);
	}

	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_) {
		enemyBullet->Draw(mainCamera_);
	}

	//敵の描画
	//enemy_->Draw(mainCamera_);
	Matrix4x4 billbordMatrix = mainCamera_->MakeBillBoardMatrix();
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		enemy->Draw(mainCamera_);
		enemy->HPGaugeDraw(mainCamera_, billbordMatrix);
	}

	skydome_->CameraUpdate(mainCamera_);
	skydome_->Draw();

	ground_->CameraUpdate(mainCamera_);
	ground_->Draw();

	ParticleManager::GetInstance()->Draw();
	
	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	player_->HUDDraw();

	fade_->Draw();

	

}

void GameScene::Finalize()
{

}

void GameScene::AddPlayerBullet(const Vector3& velocity)
{
	if (playerBullets_.size() >= kMaxPlayerBulletsNum_) {
		return;
	}
	PlayerBullet* newBullet = new PlayerBullet();
	newBullet->SetRadius(0.3f);
	Vector3 position = player_->GetCenterPosition();
	if (player_->GetLRDirection() == Collider::LRDirection::kRight) {
		newBullet->Initialize(modelPlayerBullet_.get(), {position.x + 0.5f, position.y, position.z}, velocity);
	}
	else {
		newBullet->Initialize(modelPlayerBullet_.get(), { position.x - 0.5f, position.y, position.z }, velocity);
	}
	playerBullets_.emplace_back();
	std::unique_ptr<PlayerBullet>& playerBullet = playerBullets_.back();
	playerBullet.reset(newBullet);
	audio_->SoundPlayWave(ShotSE1_, 0.7f);
}

void GameScene::AddEnemyBullet(const Vector3& position, const Vector3& velocity)
{
	std::unique_lock<std::mutex> lock(mutex_);
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(modelEnemyBullet_.get(), position, velocity);
	enemyBullets_.emplace_back();
	std::unique_ptr<EnemyBullet>& enemyBullet = enemyBullets_.back();
	enemyBullet.reset(newBullet);
}

void GameScene::CreateLevel()
{
	LevelData* levelData;
	if (stageNum_ == 1) {
		levelData = LevelDataLoad("./resources/LevelData/", "levelData02", ".json");
	}
	else {
		levelData = LevelDataLoad("./resources/LevelData/", "levelData", ".json");
	}

	//レベルデータからオブジェクトを生成、配置
	for (ObjectData& objectData : levelData->objects) {
		if (objectData.fileName == "Enemy01") {
			enemies_.emplace_back();
			std::unique_ptr<BaseEnemy>& enemy = enemies_.back();
			enemy = std::make_unique<Enemy01>();
			enemy->Initialize(modelEnemy01_.get(), objectData.transform);
			enemy->SetHPGaugeModel(modelHPGauge_.get(), modelHPGaugeFrame_.get());
			enemy->SetDarkRed(textureHandleDarkRed_);
		}
		else if (objectData.fileName == "Enemy02") {
			enemies_.emplace_back();
			std::unique_ptr<BaseEnemy>& enemy = enemies_.back();
			enemy = std::make_unique<Enemy02>();
			enemy->Initialize(modelEnemy02_.get(), objectData.transform);
			enemy->SetHPGaugeModel(modelHPGauge_.get(), modelHPGaugeFrame_.get());
			enemy->SetDarkRed(textureHandleDarkRed_);
			enemy->SetGameScene(this);

		}
		else if (objectData.fileName == "Enemy03") {
			enemies_.emplace_back();
			std::unique_ptr<BaseEnemy>& enemy = enemies_.back();
			enemy = std::make_unique<Enemy03>();
			enemy->Initialize(modelEnemy03_.get(), objectData.transform);
			enemy->SetHPGaugeModel(modelHPGauge_.get(), modelHPGaugeFrame_.get());
			enemy->SetDarkRed(textureHandleDarkRed_);
		}
		/*
		//ファイルから登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if (it != models_.end())
		{
			model = it->second;
		}
		//モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = new Object3d();
		newObject->Initialize(model, mainCamera_);

		newObject->SetTransform(objectData.transform);
		//配列に登録
		objects_.push_back(newObject);
		*/
	}
}

void GameScene::CheckAllDelete()
{
	//デスフラグの立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& playerBullet) {
		if (!playerBullet->GetIsAlive()) {
			return true;
		}
		return false;
	});

	//デスフラグの立った敵を削除
	enemies_.remove_if([](std::unique_ptr<BaseEnemy>& enemy) {
		if (!enemy->GetIsAlive()) {
			return true;
		}
		return false;
	});

	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& enemyBullet) {
		if (!enemyBullet->GetIsAlive()) {
			return true;
		}
		return false;
	});
}

void GameScene::CheckAllCollisions()
{
	threadPool_->waitForCompletion();

	Circle collider1 = { player_->Get2DCenterPosition(), player_->GetRadius() };
	Circle collider2;

	bool isSE = false;

	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		collider2 = { enemy->Get2DCenterPosition(), enemy->GetRadius() };
		if (IsCollision(collider1, collider2)) {
			player_->OnCollision();
		}
	}

	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_) {
		collider2 = { enemyBullet->Get2DCenterPosition(), enemyBullet->GetRadius() };
		if (IsCollision(collider1, collider2)) {
			enemyBullet->OnCollision();
			player_->OnCollision();
			isSE = true;
		}
	}

	for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets_) {
		collider1 = { playerBullet->Get2DCenterPosition(), playerBullet->GetRadius() };
		for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
			collider2 = { enemy->Get2DCenterPosition(), enemy->GetRadius() };
			if (IsCollision(collider1, collider2)) {
				playerBullet->OnCollision();
				enemy->OnCollision();
				isSE = true;
				emitter_->SetTranslation(enemy->GetCenterPosition());
				emitter_->Emit(blue_);
			}
		}
	}
	if (isSE) {
		audio_->SoundPlayWave(HitSE1_);
		isSE = false;
	}

	CheackPlayerAttackCollision();
}

void GameScene::CheackPlayerAttackCollision()
{
	if (!player_->GetIsAttack()) {
		return;
	}
	bool isSE3 = false;
	Square playerAttackCollider;

	if (player_->GetLRDirection() == Player::LRDirection::kRight) {
		playerAttackCollider.min.x = player_->Get2DCenterPosition().x;
		playerAttackCollider.min.y = player_->Get2DCenterPosition().y - (player_->GetAttackRange().y / 2);

		playerAttackCollider.max.x = playerAttackCollider.min.x + player_->GetAttackRange().x;
		playerAttackCollider.max.y = player_->Get2DCenterPosition().y + (player_->GetAttackRange().y / 2);
	}
	else {
		playerAttackCollider.max.x = player_->Get2DCenterPosition().x;
		playerAttackCollider.max.y = player_->Get2DCenterPosition().y + (player_->GetAttackRange().y / 2);

		playerAttackCollider.min.x = playerAttackCollider.max.x - player_->GetAttackRange().x;
		playerAttackCollider.min.y = player_->Get2DCenterPosition().y - (player_->GetAttackRange().y / 2);
	}

	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		if (IsCollision(playerAttackCollider, { enemy->Get2DCenterPosition(), enemy->GetRadius() })) {
			if (enemy->isHavingBomb()) {
				isSE3 = true;
				emitter_->SetTranslation(enemy->GetCenterPosition());
				emitter_->Emit(red_, 20);
			}
			player_->AttackHit(enemy.get(), emitter_.get());
		}
	}

	if (isSE3) {
		audio_->SoundPlayWave(HitSE3_);
		isSE3 = false;
	}
}
