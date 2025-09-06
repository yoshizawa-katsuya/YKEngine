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
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A))
	{
		phase_ = Phase::kEnd;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
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
