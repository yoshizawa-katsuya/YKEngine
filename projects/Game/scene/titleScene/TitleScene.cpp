#include "TitleScene.h"
#include "SceneManager.h"
#include "LevelDataLoader.h"
#include "RootParams.h"
#include "SceneChangeStaging.h"
#include "TitleSceneStartState.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	//UIの生成
	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Initialize();

	//ステージオブジェクトの生成
	stageObjects_ = std::make_unique<StageObjects>();
	stageObjects_->Initialize();

	CreateLevel();

	//カメラの生成
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize(railMover_->GetWorldTransform(), nullptr);

	//シーンチェンジ演出の生成
	sceneChangeStaging_ = SceneChangeStaging::GetInstance();
	sceneChangeStaging_->BeginSceneStart(StagingType::kFade);

	//ステートマシンの生成と開始
	stateMachine_ = std::make_unique<StateMachine<TitleSceneStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<TitleSceneStartState>();
}

void TitleScene::Update()
{

#ifdef USE_IMGUI
	
	ImGui::Begin("Window");
	ImGui::Text("Title");
	
	ImGui::End();
	
#endif // USE_IMGUI

	//カメラの更新
	cameraManager_->Update();

	//レールムーバーの更新
	railMover_->Update();

	//レールカメラの更新
	cameraManager_->UpdateRailCamera();

	//ステージオブジェクトの更新
	stageObjects_->Update();

	//ステートマシンの更新
	stateMachine_->Update();

	//UIの更新
	titleUI_->Update();

}

void TitleScene::Draw()
{
	Camera* mainCamera = cameraManager_->GetMainCamera();

	//背景の描画
	modelPlatform_->SkyBoxPreDraw();

	stageObjects_->DrawSkyBox(mainCamera);

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	TextureManager::GetInstance()->SetEnvironmentMap(static_cast<size_t>(ModelRootParam::kEnvironmentMap), stageObjects_->GetTextureHandleSkyBox());

	//地面の描画
	stageObjects_->Draw(mainCamera);

	modelPlatform_->InstancingPreDraw();

	//ステージオブジェクトの描画
	stageObjects_->InstancingDraw(mainCamera);

	modelPlatform_->LinePreDraw();

	railMover_->DrawRail(mainCamera);
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	// UIの描画
	titleUI_->Draw();
	
	sceneChangeStaging_->Draw();

}

void TitleScene::Finalize()
{

}

void TitleScene::UpdateEnd()
{
	if (sceneChangeStaging_->IsFinished())
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}
}

bool TitleScene::IsSceneStagingEnd() const
{
	return sceneChangeStaging_->IsFinished();
}

void TitleScene::BeginSceneEndStaging()
{
	sceneChangeStaging_->BeginSceneEnd(StagingType::kEye);
}

void TitleScene::CreateLevel()
{
	LevelData levelData;
	levelData = LevelDataLoad("./resources/LevelData/", "TitleScene", ".json");

	assert(!levelData.splines.empty());

	//レールムーバーの生成
	railMover_ = std::make_unique<RailMover>();
	railMover_->Initialize(levelData.splines[0].controlPoints, nullptr, true);

	// 背景オブジェクトの生成
	stageObjects_->GetInstancingObject(levelData.objects);
}
