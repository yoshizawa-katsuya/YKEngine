#include "GameOverScene.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//TODO : ゲームオーバー用のテクスチャに差し替える
	uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/gameOver.png");

	spriteBackGround_ = std::make_unique<Sprite>();
	spriteBackGround_->Initialize(textureHandle);

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);
}

void GameOverScene::Update()
{
#ifdef _DEBUG

	ImGui::Begin("Window");
	ImGui::Text("GameOver");
	ImGui::End();

#endif // _DEBUG

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

void GameOverScene::Draw()
{
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	spriteBackGround_->Draw();

	fade_->Draw();
}

void GameOverScene::Finalize()
{
}

void GameOverScene::UpdateStart()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void GameOverScene::UpdateMain()
{
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
		phase_ = Phase::kEnd;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
	}
}

void GameOverScene::UpdateEnd()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		//fade_->Stop();
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
}
