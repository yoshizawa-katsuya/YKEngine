#include "GameScene.h"
#include "dx12.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "JudgeSystem.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GameScene::~GameScene() {
	//Finalize();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });

	//デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	//メインカメラの設定
	mainCamera_ = camera_.get();

	//モデルを描画する際カメラの設定は必須
	//modelPlatform_->SetDirectionalLight(directionalLight_.get());
	//modelPlatform_->SetPointLight(pointLight_.get());
	modelPlatform_->SetCamera(mainCamera_);
	//modelPlatform_->SetSpotLight(spotLight_.get());

	//textureHandle_ = TextureManager::GetInstance()->Load("./resources/circle.png");
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/white.png");
	textureHandle2_ = TextureManager::GetInstance()->Load("./resources/rostock_laage_airport_4k.dds");

	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateRigidModel("./resources/Player", "Player.obj");
	//modelPlayer_->SetUVTransform({ 10.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	//modelPlayer_->SetEnableLighting(false);
	//modelPlayer_ = std::make_unique<RigidModel>();
	
	/*
	//スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	*/

	//パーティクルエミッターの生成
	//emitter_ = std::make_unique<ParticleEmitter>("Effect", 1, 1.5f);
	//emitter_->Initialize(textureHandle2, modelPlayer_, true);

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());


	//ダミーの壁の初期化
	dummyWall_=std::make_unique<DummyWall>();
	dummyWall_->Initialize(modelPlayer_.get());

	//UIの初期化
	ui_ = std::make_unique<Ui>();
	ui_->Initialize();


	/*skyBox_ = std::make_unique<Rigid3dObject>();
	skyBox_->Initialize(modelPlatform_->CreateSkyBox(textureHandle2_).get());
	skyBoxWorldTransform_.Initialize();
	skyBoxWorldTransform_.scale_ = { 50.0f, 50.0f, 50.0f };
	skyBoxWorldTransform_.UpdateMatrix();
	skyBox_->WorldTransformUpdate(skyBoxWorldTransform_);*/

	/*
	objects_ = std::make_unique<InstancingObjects>();
	objects_->Initialize(modelPlayer_.get(), 10);

	worldTransform1_.Initialize();
	worldTransform1_.translation_.x = -1.0f;
	worldTransform1_.UpdateMatrix();

	worldTransform2_.Initialize();
	worldTransform2_.translation_.x = 1.0f;
	worldTransform2_.UpdateMatrix();
	*/

	// 難易度の設定
	difficulty_ = sceneManager_->GetDifficulty();
}

void GameScene::Update() {
	

	//カメラの更新
	camera_->Update();

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	//プレイヤーの更新
	player_->Update();

	//ダミーの壁の更新
	prevWallZ_ = dummyWall_->GetWorldTransform().translation_.z;
	dummyWall_->Update();

    CheckCollision();

	//UIの更新
	ui_->Update();

	switch (ui_->GetPauseMenu()) {
		case Ui::PauseMenu::Retry:
		sceneManager_->ChengeScene("GameScene");
		break;

		case Ui::PauseMenu::ToTitle:
			sceneManager_->ChengeScene("TitleScene");
			break;
	}

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);
	/*modelPlatform_->PointLightUpdate(pointLight_);
	modelPlatform_->SpotLightUpdate(spotLight_);*/

	/*
	objects_->PreUpdate();
	worldTransform1_.translation_.x += 0.01f;
	worldTransform1_.UpdateMatrix();
	objects_->WorldTransformUpdate(worldTransform1_);
	objects_->WorldTransformUpdate(worldTransform2_);
	*/

	//emitter_->Update();

	//ParticleManager::GetInstance()->Update(mainCamera_);

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
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
		//ImGui::DragFloat3("scale", &cameratransform.scale.x, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("DirectionalLight")) {
		ImGui::ColorEdit4("color", &directionalLight_.color.x);
		ImGui::DragFloat3("direction", &directionalLight_.direction.x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_.intensity, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("PointLight")) {
		ImGui::ColorEdit4("color", &pointLight_.color.x);
		ImGui::DragFloat3("position", &pointLight_.position.x, 0.01f);
		ImGui::DragFloat("intensity", &pointLight_.intensity, 0.01f);
		ImGui::DragFloat("radius", &pointLight_.radius, 0.01f);
		ImGui::DragFloat("decay", &pointLight_.decay, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("SpotLight")) {
		ImGui::ColorEdit4("color", &spotLight_.color.x);
		ImGui::DragFloat3("position", &spotLight_.position.x, 0.01f);
		ImGui::DragFloat("intensity", &spotLight_.intensity, 0.01f);
		ImGui::DragFloat3("direction", &spotLight_.direction.x, 0.01f);
		ImGui::DragFloat("distance", &spotLight_.distance, 0.01f);
		ImGui::DragFloat("decay", &spotLight_.decay, 0.01f);
		ImGui::DragFloat("cosAngle", &spotLight_.cosAngle, 0.01f);
		ImGui::DragFloat("cosFalloffStart", &spotLight_.cosFalloffStart, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Debug")) {
		ImGui::Text("Score : %d", debugScore_);
		ImGui::Text("Miss  : %d", debugMiss_);

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
		
	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);
	ImGui::Text("Difficulty: %s", difficulty_ == 0 ? "EASY" : difficulty_ == 1 ? "NORMAL" : "HARD");
	/*
	if (ImGui::Button("BGMstop")) {
		audio_->SoundStopWave(bgm1_);
	}
	*/
	ImGui::End();
		

#endif // USE_IMGUI
	

}

void GameScene::Draw() {

	//Spriteの背景描画前処理
	spritePlatform_->PreBackGroundDraw();

	//sprite_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	//TextureManager::GetInstance()->SetEnvironmentMap(textureHandle2_);
	
	//modelPlatform_->SkinPreDraw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	//ダミーの壁の描画
	dummyWall_->Draw(mainCamera_);

	/*modelPlatform_->SkyBoxPreDraw();

	skyBox_->CameraUpdate(mainCamera_);
	skyBox_->Draw();*/

	/*
	modelPlatform_->InstancingPreDraw();

	objects_->CameraUpdate(mainCamera_);
	objects_->Draw();
	*/
	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	//UIの描画
	ui_->Draw();

	//ParticleManager::GetInstance()->Draw();

}

void GameScene::Finalize()
{

}

void GameScene::CheckCollision()
{	
	float currentZ = dummyWall_->GetWorldTransform().translation_.z;

	//判定ライン（例：z=0）
	float judgeLine = 0.0f;

	//ラインをまたいだ瞬間だけ判定
	bool crossed = (prevWallZ_ > judgeLine && currentZ <= judgeLine);

	if (!crossed) return;

	auto result = JudgeSystem::Judge(
		player_->GetState(), 
		dummyWall_->GetState(), 
		player_->GetWorldTransform(), 
		dummyWall_->GetWorldTransform()
	);

	if(result==JudgeResult::Hit){
		// 成功時の処理
		player_->SetColorForDebug(debugPlayerColor[0]);
		debugScore_++;
	}
	else if (result == JudgeResult::SuccessSquat) {
		// しゃがみ成功（デバッグ用に何か追加しても可）
	}
	else if (result == JudgeResult::Miss) {
		// ミス時の処理
		player_->SetColorForDebug(debugPlayerColor[1]);
		debugMiss_++;
	}
}
