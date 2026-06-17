#include "ClearScene.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "SceneManager.h"
#include "LevelDataLoader.h"
#include "ParticleManager.h"
#include "RootParams.h"
#include "manager/EffectManager.h"
#include "SceneChangeStaging.h"
#include "ClearSceneStartState.h"
#include "manager/CollisionManager.h"
#include "manager/AudioManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

ClearScene::~ClearScene()
{
	//衝突マネージャに登録されたコライダーを全て削除
	CollisionManager::GetInstance()->Reset();
	//BGMの停止
	audioManager_->StopBGM(BGMType::kClear);
}

void ClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//オーディオマネージャの取得
	audioManager_ = AudioManager::GetInstance();
	//BGMの再生
	audioManager_->PlayBGM(BGMType::kClear);

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	clearUI_ = std::make_unique<ClearUI>();
	clearUI_->Initialize();

	//ステージオブジェクトの生成
	stageObjects_ = std::make_unique<StageObjects>();
	stageObjects_->Initialize(true);

	CreateLevel();

	//カメラの生成
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->InitializeForClearScene(railMover_->GetWorldTransform());

	//シーンチェンジ演出の生成
	sceneChangeStaging_ = SceneChangeStaging::GetInstance();
	sceneChangeStaging_->BeginSceneStart(StagingType::kEye);

	//ステートマシンの生成
	stateMachine_ = std::make_unique<StateMachine<ClearSceneStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<ClearSceneStartState>();
}

void ClearScene::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("Window");
	ImGui::Text("Clear");
	
	ImGui::End();

#endif // USE_IMGUI

	clearUI_->Update();

	cameraManager_->Update();

	EffectManager::GetInstance()->SpawnEffect(EffectType::kConfetti01, railMover_->GetWorldTransform()->GetWorldPosition());

	//レールムーバーの更新
	railMover_->Update();

	// デモ用プレイヤーの更新
	demoPlayer_->Update();

	//レールカメラの更新
	cameraManager_->UpdateRailCamera();

	//ステージオブジェクトの更新
	stageObjects_->Update();

	//ステートマシンの更新
	stateMachine_->Update();

	ParticleManager::GetInstance()->Update(cameraManager_->GetMainCamera());

}

void ClearScene::Draw()
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
	//環境マップを使う場合はコメントアウトを外す
	TextureManager::GetInstance()->SetEnvironmentMap(static_cast<size_t>(ModelRootParam::kEnvironmentMap), stageObjects_->GetTextureHandleSkyBox());

	//地面の描画
	stageObjects_->Draw(mainCamera);

	modelPlatform_->SkinPreDraw();

	//デモ用プレイヤーの描画
	demoPlayer_->Draw(mainCamera);

	modelPlatform_->LinePreDraw();

	railMover_->DrawRail(mainCamera);

	//パーティクルの描画
	ParticleManager::GetInstance()->Draw();

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	clearUI_->Draw();

	sceneChangeStaging_->Draw();
}

void ClearScene::Finalize()
{
}

void ClearScene::UpdateEnd()
{
	if (sceneChangeStaging_->IsFinished())
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
}

void ClearScene::CreateLevel()
{
	std::unique_ptr<LevelDataLoader> levelDataLoader = std::make_unique<LevelDataLoader>();
	levelDataLoader->LevelDataLoad("./resources/LevelData/", "ClearScene", ".json");
	const LevelData& levelData = levelDataLoader->GetLevelData();

	assert(!levelData.splines.empty());

	//レールムーバーの生成
	railMover_ = std::make_unique<RailMover>();
	railMover_->Initialize(levelData.splines[0].controlPoints, nullptr, true);
	railMover_->Update();

	//デモ用プレイヤーの生成
	demoPlayer_ = std::make_unique<DemoPlayer>();
	demoPlayer_->Initialize(railMover_->GetWorldTransform());

	// 背景オブジェクトの生成
	stageObjects_->GetInstancingObject(levelData.objects);
}

bool ClearScene::IsEndSceneChangeStaging() const
{
	return sceneChangeStaging_->IsFinished();
}

void ClearScene::EnterEnd()
{
	sceneChangeStaging_->BeginSceneEnd(StagingType::kFade, {1.0f, 1.0f, 1.0f, 1.0f});
	demoPlayer_->SetIsSceneEnd(true);
}
