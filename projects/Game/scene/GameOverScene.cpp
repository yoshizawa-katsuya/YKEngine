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
	// 背景スプライトの生成
	backgroundSprite_ = std::make_unique<Sprite>();
	backgroundSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/gameoverBack.png"));
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
	player_->ChangeAnimation("SadPose");
	//遷移演出初期化
	transition_ = std::make_unique<Transition>();

	// 遷移演出の初期化
	transition_->Initialize();

	//画面に切り替わったと同時にフェードアウトの画面遷移を開始
	transition_->StartFadeOut(
		TextureManager::GetInstance()->Load("./resources/brickLoad.png"),
		TextureManager::GetInstance()->Load("./resources/brickMask.png"),
		1.0f,
		Transition::EasingType::EaseInSine
	);
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

	transition_->Update();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	player_->Update();
}

void GameOverScene::Draw()
{
	//Spriteの背景描画前処理
	spritePlatform_->PreBackGroundDraw();

	//背景の描画
	backgroundSprite_->Draw();

	modelPlatform_->SkinPreDraw();

	modelPlatform_->SkinPreDraw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	transition_->Draw();
}

void GameOverScene::Finalize()
{
}
