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
	titlePos_ = { 680.0f, 94.0f };
	colonPos_ = { 642.0f, 94.0f };
	titleLINKSprite_ = std::make_unique<YKEngine::Sprite>();
	titleLINKSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/Title/title_LINK.png"));
	titleLINKSprite_->SetPosition({
		titlePos_.x - 500.0f,
		titlePos_.y
		});
	titleLINKSprite_->SetColor({ 0.24f, 0.87f, 1.0f, 1.0f });
	titleLINKSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	titleColonSprite_ = std::make_unique<YKEngine::Sprite>();
	titleColonSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/Title/title_Colon.png"));
	titleColonSprite_->SetPosition({
		colonPos_ });
	titleColonSprite_->SetColor(kColonColorRed);
	titleColonSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	titleOVERSprite_ = std::make_unique<YKEngine::Sprite>();
	titleOVERSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/Title/title_OVER.png"));
	titleOVERSprite_->SetPosition({
		titlePos_.x + 500.0f,
		titlePos_.y
		});
	titleOVERSprite_->SetColor({ 1.0f, 0.36f, 0.54f, 1.0f });
	titleOVERSprite_->SetAnchorPoint({ 0.5f, 0.5f });


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

	float rot = titleColonSprite_->GetRotation();
	ImGui::SliderAngle("Colon Rotation", &rot, 0.0f, 360.0f);
	titleColonSprite_->SetRotation(rot);
	ImGui::Text("Colon Rotation: %.2f", titleColonSprite_->GetRotation());
	ImGui::End();

#endif // USE_IMGUI

	// タイトルアニメーション
	UpdateTitleAnimation();

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

void TitleScene::UpdateTitleAnimation()
{
	const float deltaTime = 1.0f / 60.0f;

	animationTimer_ += deltaTime;

	//==================================================
	// 開始演出
	//==================================================

	if (animationState_ == TitleAnimationState::kOpening)
	{
		float t = animationTimer_ / kOpeningDuration;
		t = std::clamp(t, 0.0f, 1.0f);

		float easedT = EaseOutCubic(t);

		// 左から中央へ
		float linkStartX = titlePos_.x - 500.0f;

		float linkX = std::lerp(
			linkStartX,
			titlePos_.x,
			easedT
		);

		titleLINKSprite_->SetPosition({
			linkX,
			titlePos_.y
			});

		// 右から中央へ
		float overStartX = titlePos_.x + 500.0f;

		float overX = std::lerp(
			overStartX,
			titlePos_.x,
			easedT
		);

		titleOVERSprite_->SetPosition({
			overX,
			titlePos_.y
			});

		// Colonは中央
		titleColonSprite_->SetPosition(colonPos_);
		// 1.57 → 0
		float rotation = std::lerp(
			kColonMaxRotation,
			0.0f,
			easedT
		);

		titleColonSprite_->SetRotation(rotation);

		if (t >= 1.0f)
		{
			animationState_ = TitleAnimationState::kMoving;
			animationTimer_ = 0.0f;
		}

		return;
	}

//==================================================
// titlePos_から開く → 戻る
//==================================================

	float animationT = animationTimer_ / kMoveDuration;

	animationT = std::fmod(animationT, 2.0f);

	// 0 → 1 → 2
	// 開く      閉じる

	float t = animationT;

	if (t > 1.0f)
	{
		t = 2.0f - t;
	}

	// 0 → 1 → 0
	float easedT = EaseInOutSine(t);

	// 最大距離
	float offset = kMoveDistance * easedT;

	// LINK
	titleLINKSprite_->SetPosition({
		titlePos_.x - offset,
		titlePos_.y
		});

	// OVER
	titleOVERSprite_->SetPosition({
		titlePos_.x + offset,
		titlePos_.y
		});

	//==================================================
	// Colon
	//==================================================

	// 回転
	float rotation =
		kColonMaxRotation * easedT;

	titleColonSprite_->SetRotation(rotation);

	//==================================================
	// Colonの色
	//==================================================

	if (isFirstColorAnimation_)
	{
		// 最初の1回だけ
		// 赤 → 緑 → 水色


			// 赤 → 緑
		float colorT = easedT;
		YKEngine::Vector4 color = {
			std::lerp(kColonColorRed.x, kColonColorGreen.x, colorT),
			std::lerp(kColonColorRed.y, kColonColorGreen.y, colorT),
			std::lerp(kColonColorRed.z, kColonColorGreen.z, colorT),
			std::lerp(kColonColorRed.w, kColonColorGreen.w, colorT)
		};

		titleColonSprite_->SetColor(color);


		// 1回目が終わった
		if (animationT >= 1.0f)
		{
			isFirstColorAnimation_ = false;
		}
	}
	else
	{
		// 緑 → 水色
		YKEngine::Vector4 color = {
			std::lerp(kColonColorGreen.x, kColonColorCyan.x, easedT),
			std::lerp(kColonColorGreen.y, kColonColorCyan.y, easedT),
			std::lerp(kColonColorGreen.z, kColonColorCyan.z, easedT),
			std::lerp(kColonColorGreen.w, kColonColorCyan.w, easedT)
		};

		titleColonSprite_->SetColor(color);
	}
}

float TitleScene::EaseOutCubic(float t)
{
	t = std::clamp(t, 0.0f, 1.0f);

	return 1.0f - std::pow(1.0f - t, 3.0f);
}

float TitleScene::EaseInOutSine(float t)
{
	t = std::clamp(t, 0.0f, 1.0f);

	return -(std::cos(3.14159265f * t) - 1.0f) * 0.5f;
}
