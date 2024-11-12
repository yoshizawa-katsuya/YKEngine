#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"

GameScene::~GameScene() {
	//Finalize();
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

	textureHandle_ = TextureManager::GetInstance()->Load("./resources/white.png");

	//モデルの生成
	modelPlayer_ = std::make_unique<Model>();
	modelPlayer_->Initialize(modelPlatform_);
	modelPlayer_->CreateModel("./resources/Player", "Player.obj");

	
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

}

void GameScene::Update() {

	input_->GamePadUpdate();

	/*
	input_->PushButton(XINPUT_GAMEPAD_A);
	input_->TriggerButton(XINPUT_GAMEPAD_B);
	input_->ReleaseButton(XINPUT_GAMEPAD_X);
	input_->HoldButton(XINPUT_GAMEPAD_Y);
	input_->GetLeftStickX();
	*/

	//カメラの更新
	camera_->Update();

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	//プレイヤーの更新
	player_->Update();

#ifdef _DEBUG


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
		//メインカメラの切り替え
		if (ImGui::RadioButton("gameCamera", !isActiveDebugCamera_)) {
			isActiveDebugCamera_ = false;

			mainCamera_ = camera_.get();


		}
		if (ImGui::RadioButton("DebugCamera", isActiveDebugCamera_)) {
			isActiveDebugCamera_ = true;

			mainCamera_ = camera2_.get();

		}
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
	spritePlatform_->PreBackGroundDraw();

	//sprite_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

}

void GameScene::Finalize()
{

}