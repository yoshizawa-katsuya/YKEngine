#include "GameClearScene.h"
#include "AudioManager.h"

#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

#include "input.h"
using namespace YKEngine;

GameClearScene::~GameClearScene() {
	//BGMの停止
	audioManager_->StopBGM(BGMType::kClear);
}

void GameClearScene::Initialize() {
	// デバイスの初期化
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//オーディオマネージャの取得
	audioManager_ = AudioManager::GetInstance();
	//BGMの再生
	audioManager_->PlayBGM(BGMType::kClear);

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
	skySphereTransform_.Initialize();
	skySphereTransform_.scale_ = { 10.0f, 10.0f, 10.0f };

	backgroundModel_ = modelPlatform_->CreateRigidModel("./resources/Background", "wireframe.obj");
	backgroundObject_ = std::make_unique<My3dObject>();
	backgroundObject_->Initialize(backgroundModel_.get());
	backgroundObject_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	backgroundTransform_.Initialize();
	backgroundTransform_.scale_ = { 1.0f, 1.0f, 1.0f };

	gameClearPos_ = { 680.0f, 94.0f };
	// GameClearSprite_の生成
	gameClearSprite_ = std::make_unique<YKEngine::Sprite>();
	gameClearSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/GameClear/GameClear.png"));
	gameClearSprite_->SetPosition(gameClearPos_);
	gameClearSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	returnPos_ = { 540.0f, 541.0f };
	returnSprite_ = std::make_unique<YKEngine::Sprite>();
	returnSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/GameOver/Title.png"));
	returnSprite_->SetPosition(returnPos_);
	returnSprite_->SetColor({ 0.9f, 0.9f, 0.9f, 1.0f });

	sceneTransition_.Outro("./resources/white.png");
}

void GameClearScene::Update()
{
	// 遷移演出が終わったらシーンを切り替える
	if (sceneTransition_.ConsumeFinished())
	{
		sceneManager_->ChengeScene(
			sceneTransition_.GetTargetScene()
		);

		return;
	}
	
	UpdateDecide();

	sceneTransition_.Update();

	skySphereTransform_.UpdateMatrix();

	skySphereObject_->WorldTransformUpdate(skySphereTransform_);

	backgroundTransform_.rotation_.x += 0.001f;
	backgroundTransform_.rotation_.y += 0.001f;

	backgroundTransform_.UpdateMatrix();

	backgroundObject_->WorldTransformUpdate(backgroundTransform_);

	// 経過時間を加算
	pressKeyTimer_ += 1.0f / 60.0f;

	// sinカーブでアルファ値を0.3〜1.0の間で滑らかに往復させる
	float t = sinf(pressKeyTimer_ * kFlickerSpeed);
	float alpha = kAlphaFlickerMin + (kAlphaStable - kAlphaFlickerMin) * (t * 0.5f + 0.5f);

	// 色はそのまま、アルファだけ変化させる
	returnSprite_->SetColor({ kColorPressKey.x, kColorPressKey.y, kColorPressKey.z, alpha });

	DrawImGui();
}

void GameClearScene::Draw()
{
	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	//TextureManager::GetInstance()->SetEnvironmentMap(textureHandle2_);
	// 天球の描画
	skySphereObject_->CameraUpdate(mainCamera_);
	skySphereObject_->Draw();

	backgroundObject_->CameraUpdate(mainCamera_);
	backgroundObject_->Draw();
	
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	gameClearSprite_->Draw();
	returnSprite_->SetPosition(returnPos_);
	returnSprite_->Draw();
	sceneTransition_.Draw();
}

void GameClearScene::DrawImGui()
{
	// ImGuiの描画
#ifdef USE_IMGUI
	ImGui::Begin("SELECT");
	ImGui::Text("-> Title");
	ImGui::SliderFloat2("titlePos", &returnPos_.x, 0.0f, 1280.0f);
	ImGui::End();
#endif
}

void GameClearScene::Finalize()
{
}



void GameClearScene::UpdateDecide()
{
	Input* input = Input::GetInstance();

	// 決定ボタン（スペース）
	bool decide = input->TriggerKey(DIK_SPACE);

	if (!decide) {
		return;
	}

	// 以降の入力を無視する（多重入力防止）
	isDecided_ = true;
	// 遷移演出開始
	if(!sceneTransition_.IsTransitioning() && !sceneTransition_.IsFinished())
	{
		sceneTransition_.Intro("TitleScene", "./resources/white.png");
		audioManager_->PlaySE(SEType::kDecision01);
	}
	
}
