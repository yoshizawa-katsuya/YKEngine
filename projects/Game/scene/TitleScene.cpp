#include "TitleScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

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
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);
}

void TitleScene::Update()
{

#ifdef _DEBUG
	
	ImGui::Begin("Window");
	ImGui::Text("Title");
	ImGui::End();
	
#endif // _DEBUG

	switch (phase_)
	{
	case TitleScene::Phase::kStart:

		UpdateStart();
		break;
	case TitleScene::Phase::kMain:

		UpdateMain();
		break;
	case TitleScene::Phase::kEnd:

		UpdateEnd();
		break;
	default:
		break;
	}

}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	fade_->Draw();
}

void TitleScene::Finalize()
{

}

void TitleScene::UpdateStart()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void TitleScene::UpdateMain()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		phase_ = Phase::kEnd;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
	}
}

void TitleScene::UpdateEnd()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		//fade_->Stop();
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}
}
