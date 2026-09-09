#include "GameScene.h"
#include "dx12.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "NormalEnemy.h"
#include "SwiftEnemy.h"
#include "TankEnemy.h"
#include "LevelDataLoader.h"
#include "AudioManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GameScene::~GameScene() 
{
	//衝突マネージャーに登録されたコライダーを削除
	collisionManager_->Reset();
	//BGMの停止
	audioManager_->StopBGM(BGMType::kGame);
}

void GameScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//オーディオマネージャの取得
	audioManager_ = AudioManager::GetInstance();
	//BGMの再生
	audioManager_->PlayBGM(BGMType::kGame);

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -20.0f });

	//デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	//メインカメラの設定
	mainCamera_ = camera_.get();

	//モデルを描画する際カメラの設定は必須
	modelPlatform_->SetCamera(mainCamera_);
	
	//プレイヤー管理クラスの生成
	playerManager_ = std::make_unique<PlayerManager>();
	playerManager_->Initialize();
	
	//衝突マネージャーの取得
	collisionManager_ = CollisionManager::GetInstance();
	//modelPlatform_->SetSpotLight(spotLight_.get());

	//ステージの生成
	CreateLevel();

	/*std::unique_ptr<BaseEnemy> enemy = std::make_unique<SwiftEnemy>();
	std::unique_ptr<BaseEnemy> enemy = std::make_unique<TankEnemy>();
	enemy->Initialize();
	enemy->SetTargets(playerManager_->GetLeftPlayer(), playerManager_->GetRightPlayer());
	enemies_.push_back(std::move(enemy));*/

	// スプライトの生成
	leftPlayerIcon_ = std::make_unique<Sprite>();
	leftPlayerIcon_->Initialize(TextureManager::GetInstance()->Load("./resources/leftPlayerIcon.png"));
	rightPlayerIcon_ = std::make_unique<Sprite>();
	rightPlayerIcon_->Initialize(TextureManager::GetInstance()->Load("./resources/rightPlayerIcon.png"));
	moveSprite_ = std::make_unique<Sprite>();
	moveSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/move.png"));
	wasdSprite_ = std::make_unique<Sprite>();
	wasdSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/wasd.png"));
	arrowKeySprite_ = std::make_unique<Sprite>();
	arrowKeySprite_->Initialize(TextureManager::GetInstance()->Load("./resources/arrow_Key.png"));
	energyLabelSprite_ = std::make_unique<Sprite>();
	energyLabelSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/energy.png"));
	energyLabelSprite_->SetSize({ 80.0f,30.0f });
	leftPlayerIconPos_ = { 10.0f, 600.0f };
	rightPlayerIconPos_ = { 800.0f, 600.0f };
	moveSpritePos_ = { 10.0f, 500.0f };
	wasdSpritePos_ = { 100.0f, 600.0f };
	arrowKeySpritePos_ = { 900.0f, 600.0f };

	// 背景モデルの生成
	skySphereModel_ = modelPlatform_->CreateRigidModel("./resources/Background", "background.obj");
	skySphereObject_ = std::make_unique<My3dObject>();
	skySphereObject_->Initialize(skySphereModel_.get());
	skySphereTransform_.Initialize();
	skySphereTransform_.scale_ = { 10.0f, 10.0f, 10.0f };

	backgroundModel_ = modelPlatform_->CreateRigidModel("./resources/Background", "wireframe.obj");
	backgroundObject_ = std::make_unique<My3dObject>();
	backgroundObject_->Initialize(backgroundModel_.get());
	backgroundObject_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	backgroundTransform_.Initialize();
	backgroundTransform_.scale_ = { 1.0f, 1.0f, 1.0f };

	energyLabelSpritePos_ = { 75.0f, 56.0f };
	sceneTransition_.Outro("./resources/white.png");
}

void GameScene::Update() {

	//カメラの更新
	camera_->Update();

	if (isActiveDebugCamera_)
	{
		debugCamera_->Update();
	}

	// 遷移演出が終わったらシーンを切り替える
	if (sceneTransition_.ConsumeFinished())
	{
		sceneManager_->ChengeScene(
			sceneTransition_.GetTargetScene()
		);

		return;
	}

	sceneTransition_.Update();

	ParticleManager::GetInstance()->Update(mainCamera_);

	//タイマーの更新
	timer_ += 1.0f / 60.0f;

	//プレイヤーの更新
	playerManager_->Update();

	//敵が死亡していたらコライダーを削除して敵のインスタンスを破棄する
	enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(),
		[&](const std::unique_ptr<BaseEnemy>& enemy) {
			if (!enemy->IsAlive()) {
				collisionManager_->RemoveSphereCollider(enemy.get());
				return true; // 削除する
			}
			return false; // 残す
		}), enemies_.end());
	//敵の更新
	for (const std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		enemy->Update();
	}

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);


	skySphereTransform_.UpdateMatrix();

	skySphereObject_->WorldTransformUpdate(skySphereTransform_);

	backgroundTransform_.rotation_.x += 0.001f;
	backgroundTransform_.rotation_.y += 0.001f;

	backgroundTransform_.UpdateMatrix();

	backgroundObject_->WorldTransformUpdate(backgroundTransform_);
	
	//衝突判定
	collisionManager_->CheckAllCollisions();

	//敵が全滅したか、タイマーが60秒を超えたらゲームクリアシーンに遷移する
	if (enemies_.empty() || timer_ > 60.0f)  
	{
		// 遷移演出開始
		if (!sceneTransition_.IsTransitioning() && !sceneTransition_.IsFinished())
		{
			sceneTransition_.Intro("GameClearScene", "./resources/white.png");
		}
		//sceneManager_->ChengeScene("GameClearScene");
	}
	if (playerManager_->GetHp() <= 0)
	{
		if (!sceneTransition_.IsTransitioning() && !sceneTransition_.IsFinished())
		{
			sceneTransition_.Intro("GameOverScene", "./resources/white.png");
		}
		//sceneManager_->ChengeScene("GameOverScene");
	}

#ifdef USE_IMGUI


	ImGui::Begin("Window");
	if (ImGui::TreeNode("camera")) {
		Vector3 translate = camera_->GetTranslate();
		ImGui::DragFloat3("translate", &translate.x, 0.01f);
		camera_->SetTranslate(translate);

		Vector3 rotate = camera_->GetRotate();
		ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
		camera_->SetRotate(rotate);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("DirectionalLight")) {
		ImGui::ColorEdit4("color", &directionalLight_.color.x);
		ImGui::DragFloat3("direction", &directionalLight_.direction.x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_.intensity, 0.01f);

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

		mainCamera_ = debugCamera_->GetCamera();
		modelPlatform_->SetCamera(mainCamera_);

	}
	
	ImGui::DragFloat2("leftPlayerIconPos", &leftPlayerIconPos_.x, 0.1f);
	ImGui::DragFloat2("rightPlayerIconPos", &rightPlayerIconPos_.x, 0.1f);
	ImGui::DragFloat2("moveSpritePos", &moveSpritePos_.x, 0.1f);
	ImGui::DragFloat2("wasdSpritePos", &wasdSpritePos_.x, 0.1f);
	ImGui::DragFloat2("arrowKeySpritePos", &arrowKeySpritePos_.x, 0.1f);
	ImGui::DragFloat2("energyLabelSpritePos", &energyLabelSpritePos_.x, 0.1f);
	
		
	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);

	
	ImGui::End();
		

#endif // USE_IMGUI
	

}

void GameScene::Draw() {

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	//TextureManager::GetInstance()->SetEnvironmentMap(textureHandle2_);
	// 天球の描画
	skySphereObject_->CameraUpdate(mainCamera_);
	skySphereObject_->Draw();

	backgroundObject_->CameraUpdate(mainCamera_);
	backgroundObject_->Draw();

	//プレイヤーの描画
	playerManager_->Draw(mainCamera_);

	//敵の描画
	for (const std::unique_ptr<BaseEnemy>& enemy : enemies_) {
		enemy->Draw(mainCamera_);
	}

	
	modelPlatform_->InstancingPreDraw();

	//コライダーの描画
	collisionManager_->Draw(mainCamera_);

	ParticleManager::GetInstance()->Draw();

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	//HUDの描画
	playerManager_->DrawHUD();

	//プレイヤーアイコンの描画
	leftPlayerIcon_->SetPosition(leftPlayerIconPos_);
	rightPlayerIcon_->SetPosition(rightPlayerIconPos_);
	moveSprite_->SetPosition(moveSpritePos_);
	wasdSprite_->SetPosition(wasdSpritePos_);
	arrowKeySprite_->SetPosition(arrowKeySpritePos_);
	energyLabelSprite_->SetPosition(energyLabelSpritePos_);
	leftPlayerIcon_->Draw();
	rightPlayerIcon_->Draw();
	moveSprite_->Draw();
	wasdSprite_->Draw();
	arrowKeySprite_->Draw();
	energyLabelSprite_->Draw();

	sceneTransition_.Draw();
}

void GameScene::Finalize()
{

}

void GameScene::CreateLevel()
{
	LevelData levelData = LevelDataLoad("./Resources/LevelData/", "Level1", ".json");

	for (const EnemySpawnData& enemySpawn : levelData.enemySpawns) {
		std::unique_ptr<BaseEnemy> enemy;
		if (enemySpawn.type == "NormalEnemy") 
		{
			enemy = std::make_unique<NormalEnemy>();
		}
		else if (enemySpawn.type == "SwiftEnemy")
		{
			enemy = std::make_unique<SwiftEnemy>();
		}
		else if (enemySpawn.type == "TankEnemy")
		{
			enemy = std::make_unique<TankEnemy>();
		}
		else 
		{
			assert(false && "Unknown enemy type");
		}
		enemy->Initialize(enemySpawn, playerManager_->GetLeftPlayer(), playerManager_->GetRightPlayer());
		enemies_.push_back(std::move(enemy));
	}
}
