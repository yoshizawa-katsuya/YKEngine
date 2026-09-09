#include "GameOverScene.h"
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

#include "input.h"
using namespace YKEngine;

GameOverScene::~GameOverScene() {
}

void GameOverScene::Initialize() {
	// デバイスの初期化
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	// 状態初期化
	select_ = SELECT::kRetry;
	isDecided_ = false;

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -20.0f });

	//メインカメラの設定
	mainCamera_ = camera_.get();

	//モデルを描画する際カメラの設定は必須
	modelPlatform_->SetCamera(mainCamera_);

	// 背景モデルの生成
	skySphereModel_ = modelPlatform_->CreateRigidModel("./resources/Background", "background.obj");
	skySphereObject_ = std::make_unique<My3dObject>();
	skySphereObject_->Initialize(skySphereModel_.get());
	skySphereObject_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	skySphereTransform_.Initialize();
	skySphereTransform_.scale_ = { 10.0f, 10.0f, 10.0f };

	linkLostPos_ = { 680.0f, 94.0f };
	// LinkLostSprite_の生成
	linkLostSprite_ = std::make_unique<YKEngine::Sprite>();
	linkLostSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/GameOver/LinkLost.png"));
	linkLostSprite_->SetPosition(linkLostPos_);
	linkLostSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	retryPos_ = { 350.0f, 541.0f };
	retrySprite_ = std::make_unique<YKEngine::Sprite>();
	retrySprite_->Initialize(TextureManager::GetInstance()->Load("./resources/GameOver/Retry.png"));
	retrySprite_->SetPosition(retryPos_);
	retrySprite_->SetAnchorPoint({ 0.5f, 0.5f });

	titlePos_ = { 950.0f, 541.0f };
	titleSprite_ = std::make_unique<YKEngine::Sprite>();
	titleSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/GameOver/Title.png"));
	titleSprite_->SetPosition(titlePos_);
	titleSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	sceneTransition_.Outro("./resources/white.png");
}

void GameOverScene::Update()
{
	// 遷移演出が終わったらシーンを切り替える
	if (sceneTransition_.ConsumeFinished())
	{
		sceneManager_->ChengeScene(
			sceneTransition_.GetTargetScene()
		);

		return;
	}
	
	sceneTransition_.Update();

	skySphereTransform_.UpdateMatrix();

	skySphereObject_->WorldTransformUpdate(skySphereTransform_);

	UpdateSelect();
	UpdateDecide();

	UpdateSelectSprite();

	DrawImGui();
}

void GameOverScene::Draw()
{
	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	//TextureManager::GetInstance()->SetEnvironmentMap(textureHandle2_);
	// 天球の描画
	skySphereObject_->CameraUpdate(mainCamera_);
	skySphereObject_->Draw();

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	linkLostSprite_->Draw();
	retrySprite_->SetPosition(retryPos_);
	titleSprite_->SetPosition(titlePos_);
	retrySprite_->Draw();
	titleSprite_->Draw();
	sceneTransition_.Draw();
	
}

void GameOverScene::DrawImGui()
{
	// ImGuiの描画
#ifdef USE_IMGUI
	ImGui::Begin("SELECT");
	// 選択肢の表示
	if (select_ == SELECT::kRetry) {
		ImGui::Text("-> Retry");
		ImGui::Text("   Title");
	}
	else if (select_ == SELECT::kTitle) {
		ImGui::Text("   Retry");
		ImGui::Text("-> Title");
	}
	ImGui::SliderFloat2("retryPos", &retryPos_.x, 0.0f, 1280.0f);
	ImGui::SliderFloat2("titlePos", &titlePos_.x, 0.0f, 1280.0f);
	ImGui::End();
#endif
}

void GameOverScene::Finalize()
{
}

void GameOverScene::UpdateSelect()
{
	Input* input = Input::GetInstance();

	// 左右キーどちらかが押されたら選択項目を切り替える
	// Trigger系（そのフレームで押した瞬間だけtrue）を使い、連続切り替わりを防ぐ
	bool pressedLeft = input->TriggerKey(DIK_LEFT);
	bool pressedRight = input->TriggerKey(DIK_RIGHT);

	if (pressedLeft || pressedRight) {
		// 選択肢は2つしかないので、左右どちらでもトグルでOK
		if (select_ == SELECT::kRetry) {
			select_ = SELECT::kTitle;
		}
		else {
			select_ = SELECT::kRetry;
		}
	}
}

void GameOverScene::UpdateDecide()
{
	Input* input = Input::GetInstance();

	// 決定ボタン（スペース）
	bool decide = input->TriggerKey(DIK_SPACE);

	if (!decide) {
		return;
	}

	// 以降の入力を無視する（多重入力防止）
	isDecided_ = true;

	// 選択結果に応じてシーン遷移
	switch (select_) {
	case SELECT::kRetry:
		// リトライ：ゲームプレイシーンへ
		// 遷移演出開始
		if (!sceneTransition_.IsTransitioning() && !sceneTransition_.IsFinished())
		{
			sceneTransition_.Intro("GameScene", "./resources/white.png");
		}
		//sceneManager_->ChengeScene("GameScene");
		break;

	case SELECT::kTitle:
		// タイトルシーンへ戻る
		if (!sceneTransition_.IsTransitioning() && !sceneTransition_.IsFinished())
		{
			sceneTransition_.Intro("TitleScene", "./resources/white.png");
		}
		//sceneManager_->ChengeScene("TitleScene");
		break;
	}
}

void GameOverScene::UpdateSelectSprite()
{
	if (isDecided_)
	{
		return;
	}
	constexpr float deltaTime = 1.0f / 60.0f;

	selectFlickerTimer_ += deltaTime;

	// 0～1を往復する
	float t = std::sinf(selectFlickerTimer_ * kSelectFlickerSpeed);
	t = t * 0.5f + 0.5f;

	float alpha =
		std::lerp(
			kSelectFlickerMinAlpha,
			kSelectFlickerMaxAlpha,
			t
		);

	if (select_ == SELECT::kRetry)
	{
		// Retry：点滅
		retrySprite_->SetColor({
			1.0f,
			1.0f,
			1.0f,
			alpha
			});

		// Title：グレー
		titleSprite_->SetColor({
			0.5f,
			0.5f,
			0.5f,
			1.0f
			});
	}
	else
	{
		// Retry：グレー
		retrySprite_->SetColor({
			0.5f,
			0.5f,
			0.5f,
			1.0f
			});

		// Title：点滅
		titleSprite_->SetColor({
			1.0f,
			1.0f,
			1.0f,
			alpha
			});
	}
}
