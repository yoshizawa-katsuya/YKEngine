#include "GameOverScene.h"
#include "SceneManager.h"

GameOverScene::~GameOverScene()
{
	audio_->SoundStopWave(bgm1_);
}

void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	textureHandle_ = TextureManager::GetInstance()->Load("./resources/GameOver.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_);

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);

	bgm1_ = audio_->LoopSoundLoadWave("./resources/sound/gameOverScene.wav");
	audio_->SoundLoopPlayWave(bgm1_, 0.5f);

	DecideSE1_ = audio_->SoundLoadWave("./resources/sound/DecideSE1.wav");

}

void GameOverScene::Update()
{
	switch (phase_) {
	case GameOverScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kMain;
		}
		break;

	case GameOverScene::Phase::kMain:
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {

			fade_->Start(Fade::Status::FadeOut, 0.5f);
			phase_ = Phase::kFadoOut;
			audio_->SoundPlayWave(DecideSE1_);

		}
		break;

	case GameOverScene::Phase::kFadoOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			//fade_->Stop();
			//シーン切り替え依頼
			sceneManager_->ChengeScene("TitleScene");
		}
		break;
	}
}

void GameOverScene::Draw()
{
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	sprite_->Draw();

	fade_->Draw();

}

void GameOverScene::Finalize()
{
}
