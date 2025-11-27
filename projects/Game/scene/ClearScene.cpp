#include "ClearScene.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "SceneManager.h"
#include "LevelDataLoader.h"
#include "ParticleManager.h"
#include "RootParams.h"
#include "manager/EffectManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });

	//デバッグカメラの生成
	camera2_ = std::make_unique<Camera>();
	camera2_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera2_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(camera2_.get(), input_);

	//メインカメラの設定
	mainCamera_ = camera_.get();

	//モデルを描画する際カメラの設定は必須
	modelPlatform_->SetCamera(mainCamera_);

	//texture読み込み
	uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/clear.png");
	uint32_t textureHandleSceneChange = TextureManager::GetInstance()->Load("./Resources/SceneChange01_sheet.png");
	textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox.dds");

	spriteBackGround_ = std::make_unique<Sprite>();
	spriteBackGround_->Initialize(textureHandle);

	spriteSceneChange_ = std::make_unique<AnimatedSprite>();
	spriteSceneChange_->Initialize(textureHandleSceneChange, 20, 3);
	spriteSceneChange_->SetSize({ WinApp::kClientWidth , WinApp::kClientHeight });
	spriteSceneChange_->SetIsLoop(false);

	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateRigidModel("./Resources/player", "Player.obj");
	modelGround_ = modelPlatform_->CreateRigidModel("./Resources/ground", "Ground.obj");
	modelGround_->SetUVTransform({ {160.0f, 160.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	modelGround_->SetEnvironmentCoefficient(0.8f);

	//スカイボックスの生成
	skyBox_ = std::make_unique<Rigid3dObject>();
	skyBox_->Initialize(modelPlatform_->CreateSkyBox(textureHandleSkyBox_).get());
	WorldTransform skyBoxTransform;
	skyBoxTransform.Initialize();
	skyBoxTransform.scale_ = { 1000.0f, 1000.0f, 1000.0f };
	skyBoxTransform.UpdateMatrix();
	skyBox_->WorldTransformUpdate(skyBoxTransform);

	//地面の生成
	ground_ = std::make_unique<Rigid3dObject>();
	ground_->Initialize(modelGround_.get());
	WorldTransform groundTransform;
	groundTransform.Initialize();
	groundTransform.scale_ = { 20.0f, 20.0f, 20.0f };
	groundTransform.UpdateMatrix();
	ground_->WorldTransformUpdate(groundTransform);

	CreateLevel();
}

void ClearScene::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("Window");
	ImGui::Text("Clear");
	//メインカメラの切り替え
	if (ImGui::RadioButton("gameCamera", !isActiveDebugCamera_)) {
		isActiveDebugCamera_ = false;

		mainCamera_ = camera_.get();
		modelPlatform_->SetCamera(mainCamera_);

	}
	if (ImGui::RadioButton("DebugCamera", isActiveDebugCamera_)) {
		isActiveDebugCamera_ = true;

		mainCamera_ = camera2_.get();
		modelPlatform_->SetCamera(mainCamera_);

	}
	ImGui::End();

#endif // USE_IMGUI

	if (isActiveDebugCamera_)
	{
		debugCamera_->Update();
	}

	EffectManager::GetInstance()->SpawnEffect(EffectType::kConfetti01, railMover_->GetWorldTransform()->GetWorldPosition());

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

	ParticleManager::GetInstance()->Update(mainCamera_);

}

void ClearScene::Draw()
{
	//背景の描画
	modelPlatform_->SkyBoxPreDraw();

	skyBox_->CameraUpdate(mainCamera_);
	skyBox_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	TextureManager::GetInstance()->SetEnvironmentMap(static_cast<size_t>(ModelRootParam::kEnvironmentMap), textureHandleSkyBox_);
	
	//デモ用プレイヤーの描画
	demoPlayer_->Draw(mainCamera_);

	//地面の描画
	ground_->CameraUpdate(mainCamera_);
	ground_->Draw();

	modelPlatform_->LinePreDraw();

	railMover_->DrawRail(mainCamera_);

	//パーティクルの描画
	ParticleManager::GetInstance()->Draw();

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	spriteBackGround_->Draw();

	if (phase_ != Phase::kMain)
	{
		spriteSceneChange_->Draw();
	}
}

void ClearScene::Finalize()
{
}

void ClearScene::UpdateStart()
{
	//レールムーバーの更新
	railMover_->Update();

	// デモ用プレイヤーの更新
	demoPlayer_->Update();

	//レールカメラの更新
	railCamera_->CreateTargetRotationFromDirection(-railMover_->GetForward());
	railCamera_->Update();

	//シーン切り替えアニメーション更新
	spriteSceneChange_->Update();
	if (spriteSceneChange_->GetIsEnd())
	{
		phase_ = Phase::kMain;
	}
}

void ClearScene::UpdateMain()
{
	//レールムーバーの更新
	railMover_->Update();

	// デモ用プレイヤーの更新
	demoPlayer_->Update();

	//レールカメラの更新
	railCamera_->CreateTargetRotationFromDirection(-railMover_->GetForward());
	railCamera_->Update();

	//スペースキーまたはAボタンで終了
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A))
	{
		phase_ = Phase::kEnd;
		spriteSceneChange_->ResetReverseAnimation();
		demoPlayer_->SceneEnd();
	}
}

void ClearScene::UpdateEnd()
{
	//レールムーバーの更新
	railMover_->Update();

	// デモ用プレイヤーの更新
	demoPlayer_->Update();

	//レールカメラの更新
	railCamera_->CreateTargetRotationFromDirection(-railMover_->GetForward());
	railCamera_->Update();

	spriteSceneChange_->Update();
	if (spriteSceneChange_->GetIsEnd())
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}
}

void ClearScene::CreateLevel()
{
	LevelData* levelData;
	levelData = LevelDataLoad("./resources/LevelData/", "ClearScene", ".json");

	assert(!levelData->splines.empty());

	//レールムーバーの生成
	railMover_ = std::make_unique<RailMover>();
	railMover_->Initialize(levelData->splines[0].controlPoints, nullptr, true);
	railMover_->Update();

	// レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	// レールカメラの初期化
	railCamera_->Initialize(camera_.get(), railMover_->GetWorldTransform(), railMover_->GetWorldTransform());
	railCamera_->CreateTargetRotationFromDirection(-railMover_->GetForward());
	railCamera_->SetClearScene();

	//デモ用プレイヤーの生成
	demoPlayer_ = std::make_unique<DemoPlayer>();
	demoPlayer_->Initialize(modelPlayer_.get(), railMover_->GetWorldTransform());

}
