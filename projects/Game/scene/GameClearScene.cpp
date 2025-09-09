#include "GameClearScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);

	for (int i = 0; i < 8; i++) {
		std::string path = "Resources/scene/gameclear0" + std::to_string(i + 1) + ".png";
		gameClears_[i] = std::make_unique<Sprite>();
		gameClears_[i]->Initialize(TextureManager::GetInstance()->Load(path));
		gameClears_[i]->SetPosition({ 0.0f, 0.0f });
	}

	menuState_ = 0;
	frameCount_ = 0;
	blinkIndex_ = 0;
}

void GameClearScene::Update()
{
#ifdef _DEBUG

	ImGui::Begin("Window");
	ImGui::Text("GameClear");
	if (ImGui::Button("Go to TitleScene"))
	{
		phase_ = Phase::kEnd;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
	}
	ImGui::End();

#endif // _DEBUG

	frameCount_++;
	if (frameCount_ > 30) {
		blinkIndex_ = 1 - blinkIndex_;
		frameCount_ = 0;
	}

	switch (phase_)
	{
	case Phase::kStart:

		UpdateStart();
		break;
	case Phase::kMain:

		UpdateMain();
		break;
	case Phase::kEnd:

		UpdateEnd();
		break;
	default:
		break;
	}

}

void GameClearScene::Draw()
{
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	if (menuState_ == 0) {
		gameClears_[blinkIndex_]->Draw();
	} else if (menuState_ == 1) {
		gameClears_[2 + blinkIndex_]->Draw();
	} else if (menuState_ == 2) {
		gameClears_[4 + blinkIndex_]->Draw();
	} else if (menuState_ == 3) {
		gameClears_[6 + blinkIndex_]->Draw();
	}

	fade_->Draw();
}

void GameClearScene::Finalize()
{
}

void GameClearScene::UpdateStart()
{
	if (input_->TriggerKey(DIK_W)) {
		menuState_--;
		if (menuState_ <= 0) {
			menuState_ = 0;
		}
	}
	else if (input_->TriggerKey(DIK_S)) {
		menuState_++;
		if (menuState_ >= 3) {
			menuState_ = 3;
		}
	}

	fade_->Update();
	if (fade_->IsFinished())
	{
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void GameClearScene::UpdateMain()
{
	if (input_->TriggerKey(DIK_W)) {
		menuState_--;
		if (menuState_ <= 0) {
			menuState_ = 0;
		}
	}
	else if (input_->TriggerKey(DIK_S)) {
		menuState_++;
		if (menuState_ >= 3) {
			menuState_ = 3;
		}
	}

	if (menuState_ <= 2) {
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			phase_ = Phase::kEnd;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		}
	} else if (menuState_ == 3) {
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			PostQuitMessage(0);
		}
	}
}

void GameClearScene::UpdateEnd()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		//fade_->Stop();
		//シーン切り替え依頼
		if (menuState_ == 0)
		{
			sceneManager_->ChengeScene("RetryScene");
			return;
		}
		else if (menuState_ == 1)
		{
			sceneManager_->ChengeScene("StageSelectScene");
			return;
		}
		else if (menuState_ == 2)
		{
			sceneManager_->ChengeScene("TitleScene");
			return;
		}
	}
}
