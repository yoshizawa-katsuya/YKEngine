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

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		nextSceneName_ = "TitleScene";
		transition_->StartFadeIn(
			TextureManager::GetInstance()->Load("./resources/brickLoad.png"),
			TextureManager::GetInstance()->Load("./resources/brickMask2.png"),
			2.0f,
			Transition::EasingType::EaseOutQuint
		);
	}
}

void ClearScene::Draw()
{
	//Spriteの背景描画前処理
	spritePlatform_->PreBackGroundDraw();

	modelPlatform_->SkinPreDraw();

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	clearSprite_->Draw();

	transition_->Draw();
}

void ClearScene::Finalize()
{
}
