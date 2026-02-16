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

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	spriteBackGround_ = std::make_unique<Sprite>();
	spriteBackGround_->Initialize(TextureManager::GetInstance()->Load("./Resources/clear.png"));

	//ステージオブジェクトの生成
	stageObjects_ = std::make_unique<StageObjects>();
	stageObjects_->Initialize();

	CreateLevel();

	//カメラの生成
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->InitializeForClearScene(railMover_->GetWorldTransform(), -railMover_->GetForward());

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

	cameraManager_->Update();

	EffectManager::GetInstance()->SpawnEffect(EffectType::kConfetti01, railMover_->GetWorldTransform()->GetWorldPosition());

	//レールムーバーの更新
	railMover_->Update();

	// デモ用プレイヤーの更新
	demoPlayer_->Update();

	//レールカメラの更新
	cameraManager_->UpdateRailCamera(-railMover_->GetForward());

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

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	TextureManager::GetInstance()->SetEnvironmentMap(static_cast<size_t>(ModelRootParam::kEnvironmentMap), stageObjects_->GetTextureHandleSkyBox());
	
	//デモ用プレイヤーの描画
	demoPlayer_->Draw(mainCamera);

	//地面の描画
	stageObjects_->Draw(mainCamera);

	modelPlatform_->LinePreDraw();

	railMover_->DrawRail(mainCamera);

	//パーティクルの描画
	ParticleManager::GetInstance()->Draw();

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	spriteBackGround_->Draw();

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
	LevelData levelData;
	levelData = LevelDataLoad("./resources/LevelData/", "ClearScene", ".json");

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
	sceneChangeStaging_->BeginSceneEnd(StagingType::kFade);
	demoPlayer_->SetIsSceneEnd(true);
}
