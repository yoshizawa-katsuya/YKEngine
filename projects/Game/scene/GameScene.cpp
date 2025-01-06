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

GameScene::~GameScene() {
	//Finalize();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
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

	//モデルの生成
	modelPlayer_ = std::make_unique<RigidModel>();
	modelPlayer_->CreateModel("./resources/Player", "Player.obj");
	//modelPlayer_->CreateSphere(textureHandle_);
	
	modelHammer_ = std::make_unique<RigidModel>();
	modelHammer_->CreateModel("./resources/Hammer", "Hammer.obj");

	modelEnemy01_ = std::make_unique<RigidModel>();
	modelEnemy01_->CreateModel("./resources/enemy", "Enemy.obj");

	modelEnemy02_ = std::make_unique<RigidModel>();
	modelEnemy02_->CreateModel("./resources/enemy02", "Enemy02.obj");

	modelGround_ = std::make_unique<RigidModel>();
	modelGround_->CreateModel("./resources/ground", "Ground.obj");
	modelGround_->SetUVTransform({ 25.0f, 25.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

	modelHPGauge_ = std::make_unique<RigidModel>();
	modelHPGauge_->CreateModel("./resources/HPGauge", "HPGauge.obj");
	modelHPGauge_->SetEnableLighting(false);

	modelPlayerBullet_ = std::make_unique<RigidModel>();
	modelPlayerBullet_->CreateSphere(textureHandleBlue_);
	//modelPlayerBullet_->SetEnableLighting(false);

	ground_ = std::make_unique<Rigid3dObject>();
	ground_->Initialize(modelGround_.get());

	/*
	//テクスチャハンドルの生成
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/player/Player.png");

	//スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
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

}

void GameScene::Update() {


	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	//プレイヤーの更新
	player_->Update();

	//デスフラグの立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& playerBullet) {
		if (!playerBullet->GetIsAlive()) {
			return true;
		}
		return false;
	});

	float playerPositionX = player_->GetCenterPosition().x;
	//自キャラの弾の更新
	for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets_) {
		playerBullet->Update(playerPositionX);
	}

	//デスフラグの立った敵を削除
	enemies_.remove_if([](std::unique_ptr<BaseEnemy>& enemy) {
		if (!enemy->GetIsAlive()) {
			return true;
		}
		return false;
	});

	//敵の更新
	//enemy_->Update();
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		enemy->Update();
	}

	//カメラコントローラーの更新
	cameraController_->Update();

	//カメラの更新
	camera_->Update();

	//emitter_->Update(color_);

	//ParticleManager::GetInstance()->Update(mainCamera_, field_.get());

	CheckAllCollisions();

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
		
		/*
		if (ImGui::Button("BGMstop")) {
			audio_->SoundStopWave(bgm1_);
		}
		*/
		ImGui::End();
		/*
		ImGui::Begin("Particle");
		ImGui::ColorEdit4("color", &color_.x);
		ImGui::DragFloat3("Translate", &emitter_->GetTranslate().x, 0.01f);
		ImGui::DragFloat3("Scele", &emitter_->GetScele().x, 0.01f);
		ImGui::DragFloat3("TranslateMin", &emitter_->GetRandTranslateMin().x, 0.01f);
		ImGui::DragFloat3("TranslateMax", &emitter_->GetRandTranslateMax().x, 0.01f);
		int count = emitter_->GetCount();
		ImGui::DragInt("Count", &count, 0.1f);
		if (count < 0) {
			count = 0;
		}
		emitter_->SetCount(count);
		ImGui::DragFloat("Frequency", &emitter_->GetFrequency(), 0.01f);
		ImGui::Checkbox("IsRandomColor", &emitter_->GetIsRandomColor());
		ImGui::DragFloat3("Accerelation", &field_->accerelation.x, 0.01f);
		ImGui::DragFloat3("AccerelationFieldMin", &field_->area.min.x, 0.01f);
		ImGui::DragFloat3("AccerelationFieldMax", &field_->area.max.x, 0.01f);
		if (ImGui::Button("Reset")) {
			color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
			emitter_->SetTranslation({ 0.0f, 0.0f, 0.0f });
			emitter_->SetRandTranslationMin({ -1.0f, -1.0f, -1.0f });
			emitter_->SetRandTranslationMax({ 1.0f, 1.0f, 1.0f });
			emitter_->SetCount(10);
			emitter_->SetFrequency(0.5f);
			emitter_->SetIsRandomColor(true);
			field_->accerelation = { 5.0f, 0.0f, 0.0f };
			field_->area.max = { 1.0f, 1.0f, 1.0f };
			field_->area.min = { -1.0f, -1.0f, -1.0f };
			ParticleManager::GetInstance()->SetUseAccelerationField(false);
		}
		if (ImGui::Button("FireParticle")) {
			color_ = { 1.0f, 0.3f, 0.0f, 1.0f };
			emitter_->SetTranslation({ 0.0f, -2.0f, 0.0f });
			emitter_->SetRandTranslationMin({ -3.0f, 0.0f, -3.0f });
			emitter_->SetRandTranslationMax({ 3.0f, 0.3f, 1.0f });
			emitter_->SetCount(3);
			emitter_->SetFrequency(0.01f);
			emitter_->SetIsRandomColor(false);
			field_->accerelation = { 0.0f, 3.0f, 9.0f };
			field_->area.max = { 5.0f, 0.0f, 5.0f };
			field_->area.min = { -5.0f, -8.0f, -6.0f };
			ParticleManager::GetInstance()->SetUseAccelerationField(true);
		}
		ImGui::End();
		*/

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

	//敵の描画
	//enemy_->Draw(mainCamera_);
	Matrix4x4 billbordMatrix = mainCamera_->MakeBillBoardMatrix();
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		enemy->Draw(mainCamera_);
		enemy->HPGaugeDraw(mainCamera_, billbordMatrix);
	}

	ground_->CameraUpdate(mainCamera_);
	ground_->Draw();

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	player_->HUDDraw();

	//ParticleManager::GetInstance()->Draw();

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
	newBullet->Initialize(modelPlayerBullet_.get(), player_->GetCenterPosition(), velocity);
	playerBullets_.emplace_back();
	std::unique_ptr<PlayerBullet>& playerBullet = playerBullets_.back();
	playerBullet.reset(newBullet);
}

void GameScene::CreateLevel()
{
	LevelData* levelData = LevelDataLoad("./resources/LevelData/", "levelData", ".json");

	//レベルデータからオブジェクトを生成、配置
	for (ObjectData& objectData : levelData->objects) {
		if (objectData.fileName == "Enemy01") {
			enemies_.emplace_back();
			std::unique_ptr<BaseEnemy>& enemy = enemies_.back();
			enemy = std::make_unique<Enemy01>();
			enemy->Initialize(modelEnemy01_.get(), objectData.transform);
			enemy->SetHPGaugeModel(modelHPGauge_.get());
			enemy->SetDarkRed(textureHandleDarkRed_);
		}
		else if (objectData.fileName == "Enemy02") {
			enemies_.emplace_back();
			std::unique_ptr<BaseEnemy>& enemy = enemies_.back();
			enemy = std::make_unique<Enemy02>();
			enemy->Initialize(modelEnemy02_.get(), objectData.transform);
			enemy->SetHPGaugeModel(modelHPGauge_.get());
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

void GameScene::CheckAllCollisions()
{

	Circle collider1 = { player_->Get2DCenterPosition(), player_->GetRadius() };
	Circle collider2;

	for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		collider2 = { enemy->Get2DCenterPosition(), enemy->GetRadius() };
		if (IsCollision(collider1, collider2)) {
			player_->OnCollision();
		}
	}

	for (std::unique_ptr<PlayerBullet>& playerBullet : playerBullets_) {
		collider1 = { playerBullet->Get2DCenterPosition(), playerBullet->GetRadius() };
		for (std::unique_ptr<BaseEnemy>& enemy : enemies_) {
			collider2 = { enemy->Get2DCenterPosition(), enemy->GetRadius() };
			if (IsCollision(collider1, collider2)) {
				playerBullet->OnCollision();
				enemy->OnCollision();
			}
		}
	}

	CheackPlayerAttackCollision();
}

void GameScene::CheackPlayerAttackCollision()
{
	if (!player_->GetIsAttack()) {
		return;
	}

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
			player_->AttackHit(enemy.get());
		}
	}
}
