#include "ClearScene.h"
#include "TextureManager.h"
#include "SceneManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	//メインカメラの設定
	mainCamera_ = camera_.get();
	modelPlatform_->SetCamera(mainCamera_);
	
	clearSprite_ = std::make_unique<Sprite>();
	clearSprite_->Initialize(TextureManager::GetInstance()->Load("./Resources/clear.png"));
	clearSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	clearSprite_->SetPosition({ 640.0f, 360.0f });

	backgroundSprite_ = std::make_unique<Sprite>();
	backgroundSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/back.png"));

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

	//Ui
	ui_ = std::make_unique<Ui>();
	ui_->Initialize();

	// 音声
	pongSE_ = YKEngine::Audio::GetInstance()->SoundLoadWave("./resources/sound/pon.mp3");
	decideSE_ = YKEngine::Audio::GetInstance()->SoundLoadWave("./resources/sound/popi.mp3");
}

void ClearScene::Update()
{
#ifdef USE_IMGUI
	ImGui::Begin("Window");
	ImGui::Text("GameClear");
	ImGui::End();
#endif // USE_IMGUI

	//カメラの更新
	camera_->Update();

	transition_->Update();
	// 画面遷移が終わり、次のシーン名が設定されている場合はシーンを切り替える
	if (transition_->IsFinished() &&
		!nextSceneName_.empty()) {

		sceneManager_->ChengeScene(nextSceneName_);
	}

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	if (!isStartedTransition_) {
		if (input_->TriggerKey(DIK_LEFT)) {
			YKEngine::Audio::GetInstance()->SoundPlayWave(pongSE_, 0.8f);
			menuState_ = MenuState::Retry;
		}
		if (input_->TriggerKey(DIK_RIGHT)) {
			YKEngine::Audio::GetInstance()->SoundPlayWave(pongSE_, 0.8f);
			menuState_ = MenuState::Title;
		}
	}

	if (!isStartedTransition_ &&
		input_->TriggerKey(DIK_SPACE))
	{
		YKEngine::Audio::GetInstance()->SoundPlayWave(decideSE_, 0.8f);
		isStartedTransition_ = true;

		switch (menuState_) {
		case MenuState::Retry:
			nextSceneName_ = "GameScene";
			break;

		case MenuState::Title:
			nextSceneName_ = "TitleScene";
			break;
		}

		transition_->StartFadeIn(
			TextureManager::GetInstance()->Load("./resources/brickLoad.png"),
			TextureManager::GetInstance()->Load("./resources/brickMask2.png"),
			2.0f,
			Transition::EasingType::EaseOutQuint
		);
	}

	ui_->SetGameClearSelect(
		menuState_ == MenuState::Retry ? 0 : 1
	);

	//Uiの更新
	ui_->Update();
}

void ClearScene::Draw()
{
	//Spriteの背景描画前処理
	spritePlatform_->PreBackGroundDraw();

	backgroundSprite_->Draw();

	modelPlatform_->SkinPreDraw();

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	// 遷移中でなければUI描画
	if (!isStartedTransition_)
	{
		ui_->DrawGameClear();
	}

	transition_->Draw();
}

void ClearScene::Finalize()
{
}
