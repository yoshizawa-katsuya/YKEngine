#include "TitleScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

uint32_t TitleScene::selectNum_ = 1;

TitleScene::~TitleScene()
{
	//Finalize();
	audio_->SoundStopWave(bgm1_);

}

void TitleScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();
	
	textureHandleTitle_ = TextureManager::GetInstance()->Load("./resources/Title.png");
	texturehandleCursor_ = TextureManager::GetInstance()->Load("./resources/cursor.png");

	spriteTitle_ = std::make_unique<Sprite>();
	spriteTitle_->Initialize(textureHandleTitle_);

	spriteCursor_ = std::make_unique<Sprite>();
	spriteCursor_->Initialize(texturehandleCursor_);
	spriteCursor_->SetPosition({ 156.0f, 315.0f + selectNum_ * 85.0f });

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);

	bgm1_ = audio_->LoopSoundLoadWave("./resources/sound/titleScene.wav");
	audio_->SoundLoopPlayWave(bgm1_, 0.5f);

	SelectSE1_ = audio_->SoundLoadWave("./resources/sound/SelectSE1.wav");
	DecideSE1_ = audio_->SoundLoadWave("./resources/sound/DecideSE1.wav");
}

void TitleScene::Update()
{

#ifdef _DEBUG
	
	ImGui::Begin("Window");
	ImGui::Text("Title");
	ImGui::End();
	
#endif // _DEBUG

	switch (phase_) {
	case TitleScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kMain;
		}
		break;

	case TitleScene::Phase::kMain:
		if (input_->TriggerKey(DIK_W) || input_->TriggerButton(XINPUT_GAMEPAD_DPAD_UP) || input_->TrigerLeftStickUp()) {
			selectNum_--;
			if (selectNum_ < 1) {
				selectNum_ = 3;
			}
			spriteCursor_->SetPosition({ 156.0f, 315.0f + selectNum_ * 85.0f});
			audio_->SoundPlayWave(SelectSE1_);
		}
		else if (input_->TriggerKey(DIK_S) || input_->TriggerButton(XINPUT_GAMEPAD_DPAD_DOWN) || input_->TrigerLeftStickDown()) {
			selectNum_++;
			if (selectNum_ > 3) {
				selectNum_ = 1;
			}
			spriteCursor_->SetPosition({ 156.0f, 315.0f + selectNum_ * 85.0f });
			audio_->SoundPlayWave(SelectSE1_);
		}

		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			fade_->Start(Fade::Status::FadeOut, 0.5f);
			phase_ = Phase::kFadoOut;
			audio_->SoundPlayWave(DecideSE1_);
		}
		break;

	case TitleScene::Phase::kFadoOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			//fade_->Stop();
			//シーン切り替え依頼
			if (selectNum_ == 1) {
				sceneManager_->ChengeScene("TutorialScene");
			}
			else if (selectNum_ == 2) {
				sceneManager_->ChengeScene("GameScene01");
			}
			else {
				sceneManager_->ChengeScene("GameScene02");
			}
		}
		break;
	}

	
	
}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	
	spriteTitle_->Draw();

	spriteCursor_->Draw();

	fade_->Draw();
}

void TitleScene::Finalize()
{

}
