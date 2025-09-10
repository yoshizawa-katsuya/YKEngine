#include "GameOverScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"
#include "GameScene.h"

GameOverScene::~GameOverScene() {
}

void GameOverScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);

	for (int i = 0; i < 8; i++) {
		std::string path = "Resources/scene/gameover0" + std::to_string(i + 1) + ".png";
		gameOvers[i] = std::make_unique<Sprite>();
		gameOvers[i]->Initialize(TextureManager::GetInstance()->Load(path));
		gameOvers[i]->SetPosition({ 0.0f, 0.0f });
	}

	menuState = 0;
	frameCount = 0;
	blinkIndex = 0;


}

void GameOverScene::Update() {
#ifdef _DEBUG

	ImGui::Begin("Window");
	ImGui::Text("GameOver");
	if (ImGui::Button("Go to TitleScene")) {
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


	switch (phase_) {
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

void GameOverScene::Draw() {
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	if (menuState == 0) {
		gameOvers[blinkIndex]->Draw();
	} else if (menuState == 1) {
		gameOvers[2 + blinkIndex]->Draw();
	} else if (menuState == 2) {
		gameOvers[4 + blinkIndex]->Draw();
	} else if (menuState == 3) {
		gameOvers[6 + blinkIndex]->Draw();
	}

	fade_->Draw();
}

void GameOverScene::Finalize() {
}

void GameOverScene::UpdateStart() {
	fade_->Update();
	if (fade_->IsFinished()) {
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void GameOverScene::UpdateMain() {
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
		if(menuState == 0) { // Retry (同じステージをやり直す)
			const uint32_t stageNum = GameScene::stageNum_;
			nextSceneName_ = "GameScene" + std::to_string(stageNum); 
			phase_ = Phase::kEnd;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		
		}else if (menuState == 1) {
			nextSceneName_ = "StageSelectScene";
			phase_ = Phase::kEnd;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		} else if (menuState == 2) {
			nextSceneName_ = "TitleScene";
			phase_ = Phase::kEnd;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		} else if (menuState == 3) {
			PostQuitMessage(0);
		}
	}
}

void GameOverScene::UpdateEnd() {
	fade_->Update();
	if (fade_->IsFinished()) {
		if (!nextSceneName_.empty()) {
			sceneManager_->ChengeScene(nextSceneName_);
		}
	}
}
