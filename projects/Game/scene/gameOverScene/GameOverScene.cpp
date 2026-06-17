#include "GameOverScene.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "SceneManager.h"
#include "SceneChangeStaging.h"
#include "GameOverSceneStartState.h"
#include "manager/AudioManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GameOverScene::~GameOverScene()
{
	//BGMの停止
	audioManager_->StopBGM(BGMType::kGameOver);
}

void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//オーディオマネージャの取得
	audioManager_ = AudioManager::GetInstance();
	//BGMの再生
	audioManager_->PlayBGM(BGMType::kGameOver);

	//スプライトの生成
	spriteBackGround_ = std::make_unique<Sprite>();
	spriteBackGround_->Initialize(TextureManager::GetInstance()->Load("./Resources/gameOver.png"));

	//シーンチェンジ演出の生成
	sceneChangeStaging_ = SceneChangeStaging::GetInstance();
	sceneChangeStaging_->BeginSceneStart(StagingType::kFade);

	//ステートマシンの生成と開始
	stateMachine_ = std::make_unique<StateMachine<GameOverSceneStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<GameOverSceneStartState>();
}

void GameOverScene::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("Window");
	ImGui::Text("GameOver");
	ImGui::End();

#endif // USE_IMGUI

	stateMachine_->Update();
}

void GameOverScene::Draw()
{
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	spriteBackGround_->Draw();

	sceneChangeStaging_->Draw();
}

void GameOverScene::Finalize()
{
}

void GameOverScene::UpdateEnd()
{
	if (sceneChangeStaging_->IsFinished())
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
}

bool GameOverScene::IsSceneStagingEnd() const
{
	return sceneChangeStaging_->IsFinished();
}

void GameOverScene::BeginSceneEndStaging()
{
	sceneChangeStaging_->BeginSceneEnd(StagingType::kMixFade, {0.0f, 0.0f, 0.0f, 1.0f});
}
