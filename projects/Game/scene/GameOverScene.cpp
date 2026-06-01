#include "GameOverScene.h"
#include "SceneManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	dxCommon_ = YKEngine::DirectXCommon::GetInstance();
	audio_ = YKEngine::Audio::GetInstance();
	input_ = YKEngine::Input::GetInstance();
	spritePlatform_ = YKEngine::SpritePlatform::GetInstance();
	modelPlatform_ = YKEngine::ModelPlatform::GetInstance();

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	//メインカメラの設定
	mainCamera_ = camera_.get();
	modelPlatform_->SetCamera(mainCamera_);
	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateSkinModel("./resources/playerAnimation", "SadPose.gltf");
	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());
	player_->ChangeAnimation("SadPose");
	player_->SetDirectionControl(false);
	player_->SetPoseControl(false);
	player_->SetPositon(YKEngine::Vector3{ 0.0f,-2.3f,0.0f });
	player_->SetRotate(YKEngine::Vector3{ 0.0f,92.5f,0.0f });
}

void GameOverScene::Update()
{
	#ifdef USE_IMGUI
	ImGui::Begin("Window");
	ImGui::Text("GameOver");
	ImGui::End();
#endif // USE_IMGUI

	//カメラの更新
	camera_->Update();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	player_->Update();
}

void GameOverScene::Draw()
{
	modelPlatform_->SkinPreDraw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);
}

void GameOverScene::Finalize()
{
}
