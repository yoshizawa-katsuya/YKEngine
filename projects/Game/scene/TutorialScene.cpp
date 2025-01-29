#include "TutorialScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"
#include "LevelDataLoader.h"
#include "TutorialEnemy01.h"
#include "RigidModel.h"
#include "Rigid3dObject.h"

TutorialScene::~TutorialScene()
{
	audio_->SoundStopWave(bgm1_);
}

void TutorialScene::Initialize()
{

	modelPlane_ = std::make_unique<RigidModel>();
	modelPlane_->CreateModel("./resources/Plane", "plane.obj");
	modelPlane_->SetEnableLighting(false);

	GameScene::Initialize();

	player_->SetTranslation({ -18.0f, 0.0f, 0.0f });
}

void TutorialScene::Update()
{


	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	float playerPositionX = 0.0f;

	switch (phase_) {
	case GameScene::Phase::kFadeIn:

		player_->Update();

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

		//emitter_->Update(color_);

		//ParticleManager::GetInstance()->Update(mainCamera_, field_.get());

		CheckAllCollisions();

		if (player_->GetCenterPosition().x >= player_->GetMoveRengeMax()) {
			fade_->Start(Fade::Status::FadeOut, 0.5f);
			phase_ = Phase::kClear;
		}
		/*
		else if (!player_->GetIsAlive()) {
			fade_->Start(Fade::Status::FadeOut, 0.5f);
			phase_ = Phase::kGameOver;
		}
		*/
		break;

	case GameScene::Phase::kClear:
		fade_->Update();
		if (fade_->IsFinished()) {
			//fade_->Stop();
			//シーン切り替え依頼
			sceneManager_->ChengeScene("TitleScene");
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

void TutorialScene::Draw()
{

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

	for (std::unique_ptr<Plane>& plane : planes_) {
		plane->object_->CameraUpdate(mainCamera_);
		plane->object_->Draw(plane->textureHandle_);
		//plane->object_->Draw();
	}

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	player_->HUDDraw();

	fade_->Draw();

	//ParticleManager::GetInstance()->Draw();
}

void TutorialScene::CreateLevel()
{
	LevelData* levelData = LevelDataLoad("./resources/LevelData/", "tutorialLevelData", ".json");

	//レベルデータからオブジェクトを生成、配置
	for (ObjectData& objectData : levelData->objects) {
		if (objectData.fileName == "TutorialEnemy") {
			enemies_.emplace_back();
			std::unique_ptr<BaseEnemy>& enemy = enemies_.back();
			enemy = std::make_unique<TutorialEnemy01>();
			enemy->Initialize(modelEnemy01_.get(), objectData.transform);
			enemy->SetHPGaugeModel(modelHPGauge_.get(), modelHPGaugeFrame_.get());
			enemy->SetDarkRed(textureHandleDarkRed_);
			enemy->SetHitSE3Data(&HitSE3_);
		}
		else if (objectData.fileName.find("Plane") != std::string::npos) {
			planes_.emplace_back();
			std::unique_ptr<Plane>& plane = planes_.back();
			plane = std::make_unique<Plane>();
			plane->object_ = std::make_unique<Rigid3dObject>();
			plane->object_->Initialize(modelPlane_.get());
			WorldTransform worldTransform;
			worldTransform.translation_ = objectData.transform.translation;
			worldTransform.scale_ = objectData.transform.scale;
			worldTransform.rotation_ = objectData.transform.rotation;
			worldTransform.UpdateMatrix();
			plane->object_->WorldTransformUpdate(worldTransform);

			if (objectData.fileName == "Plane01") {
				plane->textureHandle_ = TextureManager::GetInstance()->Load("./resources/plane01.png");
			}
			else if (objectData.fileName == "Plane02") {
				plane->textureHandle_ = TextureManager::GetInstance()->Load("./resources/plane02.png");
			}
			else if (objectData.fileName == "Plane03") {
				plane->textureHandle_ = TextureManager::GetInstance()->Load("./resources/plane03.png");
			}
			else if (objectData.fileName == "Plane04") {
				plane->textureHandle_ = TextureManager::GetInstance()->Load("./resources/plane04.png");
			}
			else if (objectData.fileName == "Plane05") {
				plane->textureHandle_ = TextureManager::GetInstance()->Load("./resources/plane05.png");
			}
			else if (objectData.fileName == "Plane06") {
				plane->textureHandle_ = TextureManager::GetInstance()->Load("./resources/plane06.png");
			}
			else if (objectData.fileName == "Plane07") {
				plane->textureHandle_ = TextureManager::GetInstance()->Load("./resources/plane07.png");
			}
			else if (objectData.fileName == "Plane08") {
				plane->textureHandle_ = TextureManager::GetInstance()->Load("./resources/plane08.png");
			}
		}
		
	}
}
