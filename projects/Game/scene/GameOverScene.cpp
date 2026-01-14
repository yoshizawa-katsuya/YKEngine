#include "GameOverScene.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "SceneManager.h"
#include "SceneChangeStaging.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//スプライトの生成
	spriteBackGround_ = std::make_unique<Sprite>();
	spriteBackGround_->Initialize(TextureManager::GetInstance()->Load("./Resources/gameOver.png"));

	spriteGameOverKeyBoard_ = std::make_unique<Sprite>();
	spriteGameOverKeyBoard_->Initialize(TextureManager::GetInstance()->Load("./Resources/gameoverKeyboard.png"));

	//シーンチェンジ演出の生成
	sceneChangeStaging_ = SceneChangeStaging::GetInstance();
	sceneChangeStaging_->BeginSceneStart(StagingType::kFade);
}

void GameOverScene::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("Window");
	ImGui::Text("GameOver");
	ImGui::End();

#endif // USE_IMGUI

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

	if (input_->IsConnected())
	{
		//コントローラー接続時
		spriteBackGround_->Draw();
	}
	else
	{
		//キーボード操作時
		spriteGameOverKeyBoard_->Draw();
	}
	

	sceneChangeStaging_->Draw();
}

void GameOverScene::Finalize()
{
}

void GameOverScene::UpdateStart()
{
	if (sceneChangeStaging_->IsFinished())
	{
		phase_ = Phase::kMain;
	}
}

void GameOverScene::UpdateMain()
{
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) 
	{
		phase_ = Phase::kEnd;
		sceneChangeStaging_->BeginSceneEnd(StagingType::kFade);
	}
}

void GameOverScene::UpdateEnd()
{
	if (sceneChangeStaging_->IsFinished())
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
}
