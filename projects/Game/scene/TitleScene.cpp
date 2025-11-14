#include "TitleScene.h"
#include "SceneManager.h"
#include "LevelDataLoader.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

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
	
	//平行光源の生成
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_->GetDirectionalLightData());

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

	//テクスチャの読み込み
	uint32_t textureHandleTitle = TextureManager::GetInstance()->Load("./Resources/title.png");
	uint32_t textureHandleSceneChange = TextureManager::GetInstance()->Load("./Resources/SceneChange01_sheet.png");
	textureHandle_ = TextureManager::GetInstance()->Load("./Resources/white.png");
	textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox.dds");

	spriteTitle_ = std::make_unique<Sprite>();
	spriteTitle_->Initialize(textureHandleTitle);
	
	spriteSceneChange_ = std::make_unique<AnimatedSprite>();
	spriteSceneChange_->Initialize(textureHandleSceneChange, 20, 3);
	spriteSceneChange_->SetSize({ WinApp::kClientWidth , WinApp::kClientHeight });
	spriteSceneChange_->SetIsLoop(false);

	//モデルの生成
	modelGround_ = modelPlatform_->CreateRigidModel("./Resources/ground", "Ground.obj");
	modelGround_->SetUVTransform({ {160.0f, 160.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	modelGround_->SetEnvironmentCoefficient(0.8f);

	//スカイボックスの生成
	skyBox_ = std::make_unique<Rigid3dObject>();
	skyBox_->Initialize(modelPlatform_->CreateSkyBox(textureHandleSkyBox_).get());
	WorldTransform skyBoxTransform;
	skyBoxTransform.Initialize();
	skyBoxTransform.scale_ = { 100.0f, 100.0f, 100.0f };
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

void TitleScene::Update()
{

#ifdef USE_IMGUI
	
	ImGui::Begin("Window");
	ImGui::Text("Title");
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

void TitleScene::Draw()
{

	//背景の描画
	modelPlatform_->SkyBoxPreDraw();

	skyBox_->CameraUpdate(mainCamera_);
	skyBox_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	TextureManager::GetInstance()->SetEnvironmentMap(textureHandleSkyBox_);

	//地面の描画
	ground_->CameraUpdate(mainCamera_);
	ground_->Draw();

	modelPlatform_->InstancingPreDraw();

	//オブジェクトの描画
	for (const auto& [name, instancingObject] : instancingObjects_) 
	{
		instancingObject->CameraUpdate(mainCamera_);
		instancingObject->Draw();
	}

	modelPlatform_->LinePreDraw();

	railMover_->DrawRail(mainCamera_);
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	spriteTitle_->Draw();

	if (phase_ != Phase::kMain)
	{
		spriteSceneChange_->Draw();
	}

}

void TitleScene::Finalize()
{

}

void TitleScene::UpdateStart()
{
	spriteSceneChange_->Update();
	if (spriteSceneChange_->GetIsEnd())
	{
		phase_ = Phase::kMain;
	}
}

void TitleScene::UpdateMain()
{

	//レールムーバーの更新
	railMover_->Update();

	//レールカメラの更新
	railCamera_->Update();

	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
		phase_ = Phase::kEnd;
		spriteSceneChange_->ResetReverseAnimation();
	}
}

void TitleScene::UpdateEnd()
{
	spriteSceneChange_->Update();
	if (spriteSceneChange_->GetIsEnd()) 
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}
}

void TitleScene::CreateLevel()
{
	LevelData* levelData;
	levelData = LevelDataLoad("./resources/LevelData/", "TitleScene", ".json");

	assert(!levelData->splines.empty());

	//レールムーバーの生成
	railMover_ = std::make_unique<RailMover>();
	railMover_->Initialize(levelData->splines[0].controlPoints, nullptr, true);

	// レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	// レールカメラの初期化
	railCamera_->Initialize(camera_.get(), railMover_->GetWorldTransform(), nullptr);

	//オブジェクトの生成
	std::string key;

	for (const ObjectData& objectData : levelData->objects)
	{

		key = objectData.fileName;

		if (!instancingObjects_.contains(key))
		{
			instancingObjects_.emplace(key, std::make_unique<InstancingObjects>());
			//インスタンスオブジェクトの初期化
			if (key == "primitiveCube")
			{
				instancingObjects_[key]->Initialize(modelPlatform_->CreateCube(textureHandle_).get(), 128);
			}
			else if (key == "primitiveSphere")
			{
				instancingObjects_[key]->Initialize(modelPlatform_->CreateSphere(textureHandle_).get(), 128);
			}
		}
		//ワールド変換の初期化
		WorldTransform transform;
		transform.Initialize();
		transform.rotation_ = objectData.transform.rotation;
		transform.translation_ = objectData.transform.translation;
		transform.scale_ = objectData.transform.scale;
		transform.UpdateMatrix();
		//インスタンスオブジェクトにワールド変換を設定
		instancingObjects_[key]->WorldTransformUpdate(transform);
	}
}
