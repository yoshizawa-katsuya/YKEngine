#include "GameScene.h"
#include "Vector.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "LevelDataLoader.h"
#include "Matrix.h"

GameScene::~GameScene() {
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize(dxCommon_);

	modelPlatform_->SetDirectionalLight(directionalLight_.get());

	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });

	camera2_ = std::make_unique<Camera>();
	camera2_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera2_->SetTranslate({ 0.0f, 0.0f, -10.0f });

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(camera2_.get(), input_);

	mainCamera_ = camera_.get();

	//soundData1_ = audio_->SoundLoadWave("./resources/Alarm01.wav");

	model_ = std::make_unique<Model>();
	model_->Initialize(modelPlatform_);
	model_->CreateModel("./resources/player", "Player.obj");
	
	
	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(model_.get());


}

void GameScene::Update() {

	camera_->Update();

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	//プレイヤーの更新
	player_->Update();

	ImGui::Begin("Window");
	if (ImGui::TreeNode("camera")) {
		ImGui::DragFloat3("translate", &camera_->GetTranslate().x, 0.01f);
		ImGui::DragFloat3("rotate", &camera_->GetRotate().x, 0.01f);
		//ImGui::DragFloat3("scale", &cameratransform.scale.x, 0.01f);

		ImGui::TreePop();
	}
	
	if (ImGui::TreeNode("directionalLight")) {
		ImGui::ColorEdit4("color", &directionalLight_->GetColor().x);
		ImGui::DragFloat3("direction", &directionalLight_->GetDirection().x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_->GetIntensity(), 0.01f);

		ImGui::TreePop();
	}
	/*
	if (ImGui::Button("SE01")) {
		audio_->SoundPlawWave(soundData1_);
	}
	*/
	if (ImGui::RadioButton("gameCamera", !isActiveDebugCamera_)) {
		isActiveDebugCamera_ = false;

		mainCamera_ = camera_.get();

		

	}
	if (ImGui::RadioButton("DebugCamera", isActiveDebugCamera_)) {
		isActiveDebugCamera_ = true;

		mainCamera_ = camera2_.get();

	}
	ImGui::End();

	if (input_->PushKey(DIK_0)) {
		OutputDebugStringA("Hit 0\n");
	}
	if (input_->TriggerKey(DIK_1)) {
		OutputDebugStringA("Hit 1\n");
	}

}

void GameScene::Draw() {

	
	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//プレイヤーの描画
	player_->Draw(mainCamera_);

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	
}

void GameScene::Finalize()
{

}
