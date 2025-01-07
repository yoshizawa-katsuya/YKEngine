#include "ClearScene.h"
#include "SceneManager.h"

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	textureHandle_ = TextureManager::GetInstance()->Load("./resources/Clear.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_);

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);
}

void ClearScene::Update()
{
	switch (phase_) {
	case ClearScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kMain;
		}
		break;

	case ClearScene::Phase::kMain:
		if (input_->TriggerKey(DIK_SPACE)) {
			
			fade_->Start(Fade::Status::FadeOut, 0.5f);
			phase_ = Phase::kFadoOut;
			
		}
		break;

	case ClearScene::Phase::kFadoOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			//fade_->Stop();
			//シーン切り替え依頼
			sceneManager_->ChengeScene("TitleScene");
		}
		break;
	}
	
}

void ClearScene::Draw()
{
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	sprite_->Draw();

	fade_->Draw();
}

void ClearScene::Finalize()
{
}
