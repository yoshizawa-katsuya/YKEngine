#include "TitleScene.h"
#include "SceneManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

TitleScene::~TitleScene()
{
	//Finalize();
}

void TitleScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();
	/*
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/Title.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	sprite_->SetPosition({ 100.0f, 100.0f });
	*/
	titlePos_ = { 327.0f, 94.0f };
	titleLINKSprite_ = std::make_unique<YKEngine::Sprite>();
	titleLINKSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/Title/title_LINK.png"));
	titleLINKSprite_->SetPosition(titlePos_);
	titleLINKSprite_->SetColor({ 0.24f, 0.87f, 1.0f, 1.0f });

	titleColonSprite_ = std::make_unique<YKEngine::Sprite>();
	titleColonSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/Title/title_Colon.png"));
	titleColonSprite_->SetPosition({ titlePos_ });
	titleColonSprite_->SetColor({ 1.0f, 0.36f, 0.54f, 1.0f });

	titleOVERSprite_ = std::make_unique<YKEngine::Sprite>();
	titleOVERSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/Title/title_OVER.png"));
	titleOVERSprite_->SetPosition({ titlePos_ });
	titleOVERSprite_->SetColor({ 1.0f, 0.36f, 0.54f, 1.0f });

	startPos_ = { 197.0f, 541.0f };
	startSprite_ = std::make_unique<YKEngine::Sprite>();
	startSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/Title/start.png"));
	startSprite_->SetPosition(startPos_);
	startSprite_->SetColor({ 0.9f, 0.9f, 0.9f, 1.0f });
}

void TitleScene::Update()
{

#ifdef USE_IMGUI
	
	ImGui::Begin("Window");
	ImGui::Text("Title");
	ImGui::End();
	
#endif // USE_IMGUI

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}

	// 経過時間を加算
	pressKeyTimer_ += 1.0f / 60.0f;

	// sinカーブでアルファ値を0.3〜1.0の間で滑らかに往復させる
	float t = sinf(pressKeyTimer_ * kFlickerSpeed);
	float alpha = kAlphaFlickerMin + (kAlphaStable - kAlphaFlickerMin) * (t * 0.5f + 0.5f);

	// 色はそのまま、アルファだけ変化させる
	startSprite_->SetColor({ kColorPressKey.x, kColorPressKey.y, kColorPressKey.z, alpha });
}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	titleLINKSprite_->Draw();
	titleColonSprite_->Draw();
	titleOVERSprite_->Draw();
	startSprite_->Draw();
}

void TitleScene::Finalize()
{

}
