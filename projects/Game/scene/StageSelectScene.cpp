#include "StageSelectScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"
#include "GameScene.h"

StageSelectScene::~StageSelectScene() {
	Finalize();
}

void StageSelectScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);

	control = std::make_unique<Sprite>();
	control->Initialize(TextureManager::GetInstance()->Load("Resources/scene/controltile.png"));
	control->SetPosition({ 140.0f,10.0f });


	for (int i = 0; i < 24; i++) {
		char fileName[64];
		sprintf_s(fileName, "Resources/scene/select%02d.png", i + 1); // 01~09, 10~22
		selects[i] = std::make_unique<Sprite>();
		selects[i]->Initialize(TextureManager::GetInstance()->Load(fileName));
		selects[i]->SetPosition({ 0.0f, 0.0f });
	}
	menuState = { 0, 0 };
	frameCount = 0;
	blinkIndex = 0;

	menuSE_ = audio_->LoopSoundLoadWave("Resources/sound/menuidou.mp3");
	ketteiSE_ = audio_->LoopSoundLoadWave("Resources/sound/sentaku.mp3");
}

void StageSelectScene::Update() {

#ifdef _DEBUG

	ImGui::Begin("Window");
	ImGui::Text("StageSelect");
	if (ImGui::Button("Go to GameScene")) {
		phase_ = Phase::kEnd;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
	}
	ImGui::End();

#endif // _DEBUG

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

void StageSelectScene::Draw() {

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	if (menuState.x == 0) {
		selects[blinkIndex]->Draw();
	}
	else if (menuState.x == 1 && menuState.y == 0) {
		selects[2 + blinkIndex]->Draw();
	}
	else if (menuState.x == 1 && menuState.y == 1) {
		selects[4 + blinkIndex]->Draw();
	}
	else if (menuState.x == 1 && menuState.y == 2) {
		selects[6 + blinkIndex]->Draw();
	}
	else if (menuState.x == 1 && menuState.y == 3) {
		selects[8 + blinkIndex]->Draw();
	}
	else if (menuState.x == 1 && menuState.y == 4) {
		selects[10 + blinkIndex]->Draw();
	}
	else if (menuState.x == 2 && menuState.y == 0) {
		selects[12 + blinkIndex]->Draw();
	}
	else if (menuState.x == 2 && menuState.y == 1) {
		selects[14 + blinkIndex]->Draw();
	}
	else if (menuState.x == 2 && menuState.y == 2) {
		selects[16 + blinkIndex]->Draw();
	}
	else if (menuState.x == 2 && menuState.y == 3) {
		selects[18 + blinkIndex]->Draw();
	}
	else if (menuState.x == 2 && menuState.y == 4) {
		selects[20 + blinkIndex]->Draw();
	}
	else if (menuState.x == 3) {
		selects[22 + blinkIndex]->Draw();
	}
	if (showControl_) {
		control->Draw();
	}


	fade_->Draw();
}

void StageSelectScene::Finalize() 
{
	audio_->SoundStopWave(menuSE_);
	audio_->SoundStopWave(ketteiSE_);
}

void StageSelectScene::UpdateStart() {

	if (input_->TriggerKey(DIK_W)) {
		audio_->SoundPlayWave(menuSE_);

		menuState.x--;
		if (menuState.x <= 0) {
			menuState.x = 0;
		}
	}
	else if (input_->TriggerKey(DIK_S)) {
		audio_->SoundPlayWave(menuSE_);

		menuState.x++;
		if (menuState.x >= 3) {
			menuState.x = 3;
		}
	}

	if (input_->TriggerKey(DIK_A)) {
		audio_->SoundPlayWave(menuSE_);

		menuState.y--;
		if (menuState.y <= 0) {
			menuState.y = 0;
		}
	}
	else if (input_->TriggerKey(DIK_D)) {
		audio_->SoundPlayWave(menuSE_);

		menuState.y++;
		if (menuState.y >= 4) {
			menuState.y = 4;
		}
	}

	fade_->Update();
	if (fade_->IsFinished()) {
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void StageSelectScene::UpdateMain() {

	if (input_->TriggerKey(DIK_W)) {
		audio_->SoundPlayWave(menuSE_);

		menuState.x--;
		if (menuState.x <= 0) {
			menuState.x = 0;
		}
	}
	else if (input_->TriggerKey(DIK_S)) {
		audio_->SoundPlayWave(menuSE_);

		menuState.x++;
		if (menuState.x >= 3) {
			menuState.x = 3;
		}
	}

	if (input_->TriggerKey(DIK_A)) {
		audio_->SoundPlayWave(menuSE_);

		menuState.y--;
		if (menuState.y <= 0) {
			menuState.y = 0;
		}
	}
	else if (input_->TriggerKey(DIK_D)) {
		audio_->SoundPlayWave(menuSE_);

		menuState.y++;
		if (menuState.y >= 4) {
			menuState.y = 4;
		}
	}

	if (menuState.x == 0) {
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			audio_->SoundPlayWave(ketteiSE_);

			showControl_ = !showControl_;
		}
	}
	if ((menuState.x == 1 || menuState.x == 2) &&
		(menuState.y >= 0 && menuState.y <= 4)) {

		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			audio_->SoundPlayWave(ketteiSE_);

			// stageNum 
			uint32_t stageNum = static_cast<uint32_t>((menuState.x - 1) * 5 + (menuState.y + 1));

			GameScene::stageNum_ = stageNum;   // stage1 ~ stage10
			nextSceneName_ = "GameScene" + std::to_string(stageNum);
			phase_ = Phase::kEnd;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		}
	}

	if (menuState.x == 3) {
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			audio_->SoundPlayWave(ketteiSE_);

			nextSceneName_ = "TitleScene";
			phase_ = Phase::kEnd;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		}
	}
}

void StageSelectScene::UpdateEnd() {
	fade_->Update();
	if (fade_->IsFinished()) {
		if (!nextSceneName_.empty()) {
			sceneManager_->ChengeScene(nextSceneName_);
		}
	}
}
