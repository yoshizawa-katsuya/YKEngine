#include "TitleScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

TitleScene::~TitleScene()
{
	//Finalize();
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

	uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/title.png");
	textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox.dds");

	/*spriteTitle_ = std::make_unique<Sprite>();
	spriteTitle_->Initialize(textureHandle);*/
	
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

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);
}

void TitleScene::Update()
{

#ifdef _DEBUG
	
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
	
#endif // _DEBUG

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

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	/*spriteTitle_->Draw();*/

	fade_->Draw();
}

void TitleScene::Finalize()
{

}

void TitleScene::UpdateStart()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void TitleScene::UpdateMain()
{
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
		phase_ = Phase::kEnd;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
	}
}

void TitleScene::UpdateEnd()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		//fade_->Stop();
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}
}
