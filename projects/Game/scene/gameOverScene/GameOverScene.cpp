#include "GameOverScene.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "SceneManager.h"
#include "SceneChangeStaging.h"
#include "GameOverSceneStartState.h"
#include "manager/AudioManager.h"
#include "manager/CollisionManager.h"
#include "RootParams.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GameOverScene::~GameOverScene()
{
	//衝突マネージャに登録されたコライダーを全て削除
	CollisionManager::GetInstance()->Reset();
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

	//光源の設定
	modelPlatform_->LightPreUpdate();
	directionalLight_.intensity = 0.1f;
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	//ステージオブジェクトの生成
	stageObjects_ = std::make_unique<StageObjects>();
	stageObjects_->Initialize(StageType::kGameOver);

	CreateLevel();

	//カメラの生成
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize(railMover_->GetWorldTransform(), nullptr);

	//ゲームオーバーUIの生成
	gameOverUI_ = std::make_unique<GameOverUI>();
	gameOverUI_->Initialize();

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

	//UIの更新
	gameOverUI_->Update();

	cameraManager_->Update();

	//レールムーバーの更新
	railMover_->Update();

	//レールカメラの更新
	cameraManager_->UpdateRailCamera();

	//ステージオブジェクトの更新
	stageObjects_->Update();

	stateMachine_->Update();
}

void GameOverScene::Draw()
{
	Camera* mainCamera = cameraManager_->GetMainCamera();

	//背景の描画
	modelPlatform_->SkyBoxPreDraw();

	stageObjects_->DrawSkyBox(mainCamera);

	modelPlatform_->InstancingPreDraw();

	//ステージオブジェクトの描画
	stageObjects_->InstancingDraw(mainCamera);

	modelPlatform_->InstancingTriplanarPreDraw();

	//トリプラナーマッピングを使用するインスタンシングオブジェクトの描画
	stageObjects_->InstancingTriplanarDraw(mainCamera);

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う
	TextureManager::GetInstance()->SetEnvironmentMap(static_cast<size_t>(ModelRootParam::kEnvironmentMap), stageObjects_->GetTextureHandleSkyBox());

	//地面の描画
	stageObjects_->Draw(mainCamera);

	modelPlatform_->LinePreDraw();

	railMover_->DrawRail(mainCamera);

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	gameOverUI_->Draw();

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

void GameOverScene::CreateLevel()
{
	std::unique_ptr<LevelDataLoader> levelDataLoader = std::make_unique<LevelDataLoader>();
	levelDataLoader->LevelDataLoad("./resources/LevelData/", "GameOverScene", ".json");
	const LevelData& levelData = levelDataLoader->GetLevelData();

	assert(!levelData.splines.empty());

	//レールムーバーの生成
	railMover_ = std::make_unique<RailMover>();
	railMover_->Initialize(levelData.splines[0].controlPoints, nullptr, true);
	railMover_->Update();

	// 背景オブジェクトの生成
	stageObjects_->GetInstancingObject(levelData.objects);
}

bool GameOverScene::IsSceneStagingEnd() const
{
	return sceneChangeStaging_->IsFinished();
}

void GameOverScene::BeginSceneEndStaging()
{
	sceneChangeStaging_->BeginSceneEnd(StagingType::kMixFade, {0.0f, 0.0f, 0.0f, 1.0f});
}
