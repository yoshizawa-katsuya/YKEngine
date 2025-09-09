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
		gameClears[i] = std::make_unique<Sprite>();
		gameClears[i]->Initialize(TextureManager::GetInstance()->Load(path));
		gameClears[i]->SetPosition({ 0.0f, 0.0f });
	}

	menuState = 0;
	frameCount = 0;
	blinkIndex = 0;
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

	if (input_->TriggerKey(DIK_W)) {
		menuState--;
		if (menuState <= 0) {
			menuState = 0;
		}
	} else if (input_->TriggerKey(DIK_S)) {
		menuState++;
		if (menuState >= 3) {
			menuState = 3;
		}
	}

	frameCount++;
	if (frameCount > 30) {
		blinkIndex = 1 - blinkIndex;
		frameCount = 0;
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

	if (menuState == 0) {
		gameClears[blinkIndex]->Draw();
	} else if (menuState == 1) {
		gameClears[2 + blinkIndex]->Draw();
	} else if (menuState == 2) {
		gameClears[4 + blinkIndex]->Draw();
	} else if (menuState == 3) {
		gameClears[6 + blinkIndex]->Draw();
	}

	fade_->Draw();
}

void GameClearScene::Finalize()
{
}

void GameClearScene::UpdateStart()
{
	fade_->Update();
	if (fade_->IsFinished())
	{
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void GameClearScene::UpdateMain()
{
	if (menuState == 2) {
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			phase_ = Phase::kEnd;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		}
	} else if (menuState == 3) {
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
		sceneManager_->ChengeScene("TitleScene");
	}
}
