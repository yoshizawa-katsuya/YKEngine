#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"

GameScene::~GameScene() {
	//Finalize();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//平行光源の生成
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	
	//点光源の生成
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	
	//スポットライトの生成
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();

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

	//textureHandle_ = TextureManager::GetInstance()->Load("./resources/circle.png");
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/white.png");
	textureHandle2_ = TextureManager::GetInstance()->Load("./resources/rostock_laage_airport_4k.dds");
	uint32_t textureHandleBlock_ = TextureManager::GetInstance()->Load("./resources/block.png");

	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateRigidModel("./resources/Player", "Player.obj");
	modelBlock_ = modelPlatform_->CreateCube(textureHandleBlock_, "block");
	modelElectric_ = modelPlatform_->CreateRigidModel("./resources/Denki", "denkih.obj");

	CreateLevel();

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	Vector3 PlayerPosition = mapChipField_->GetMapChipPositionByIndex(4, 14);
	player_->Initialize(modelPlayer_.get(), PlayerPosition);
	player_->SetMapChipField(mapChipField_.get());
	player_->SetElectricModel(modelElectric_.get());

	//カメラコントローラーの生成
	cameraController_ = std::make_unique<CameraController>();
	cameraController_->Initialize(camera_.get(), player_.get(), mapChipField_.get());

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);

	doorGimmick_ = std::make_unique<Door>();
	doorGimmick_->Initialize();
}

void GameScene::Update() {

	if (isActiveDebugCamera_) {
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
	case Phase::kGameClear:
		UpdateGameClear();
		break;
	case Phase::kGameOver:
		UpdateGameOver();
		break;
	default:
		break;
	}

	CheckElectricCollision();

	cameraController_->Update();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_->GetDirectionalLightData());


	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}

#ifdef _DEBUG


	ImGui::Begin("Window");
	if (ImGui::TreeNode("camera")) {
		ImGui::DragFloat3("translate", &camera_->GetTranslate().x, 0.01f);
		ImGui::DragFloat3("rotate", &camera_->GetRotate().x, 0.01f);
		//ImGui::DragFloat3("scale", &cameratransform.scale.x, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("DirectionalLight")) {
		ImGui::ColorEdit4("color", &directionalLight_->GetColor().x);
		ImGui::DragFloat3("direction", &directionalLight_->GetDirection().x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_->GetIntensity(), 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("PointLight")) {
		ImGui::ColorEdit4("color", &pointLight_->GetColor().x);
		ImGui::DragFloat3("position", &pointLight_->GetPosition().x, 0.01f);
		ImGui::DragFloat("intensity", &pointLight_->GetIntensity(), 0.01f);
		ImGui::DragFloat("radius", &pointLight_->GetRadius(), 0.01f);
		ImGui::DragFloat("decay", &pointLight_->GetDecay(), 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("SpotLight")) {
		ImGui::ColorEdit4("color", &spotLight_->GetColor().x);
		ImGui::DragFloat3("position", &spotLight_->GetPosition().x, 0.01f);
		ImGui::DragFloat("intensity", &spotLight_->GetIntensity(), 0.01f);
		ImGui::DragFloat3("direction", &spotLight_->GetDirection().x, 0.01f);
		ImGui::DragFloat("distance", &spotLight_->GetDistance(), 0.01f);
		ImGui::DragFloat("decay", &spotLight_->GetDecay(), 0.01f);
		ImGui::DragFloat("cosAngle", &spotLight_->GetCosAngle(), 0.01f);
		ImGui::DragFloat("cosFalloffStart", &spotLight_->GetCosFalloffStart(), 0.01f);

		ImGui::TreePop();
	}
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
	if (ImGui::Button("Go to GameClearScene"))
	{
		phase_ = Phase::kGameClear;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
	}
	if (ImGui::Button("Go to GameOverScene"))
	{
		phase_ = Phase::kGameOver;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
	}
	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);

	/*
	if (ImGui::Button("BGMstop")) {
		audio_->SoundStopWave(bgm1_);
	}
	*/
	ImGui::End();
		

#endif // _DEBUG
	

}

void GameScene::Draw() {

	//Spriteの背景描画前処理
	//spritePlatform_->PreBackGroundDraw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	//TextureManager::GetInstance()->SetEnvironmentMap(textureHandle2_);

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	modelPlatform_->SkyBoxPreDraw();

	//modelPlatform_->SkinPreDraw();

	modelPlatform_->InstancingPreDraw();

	blocks_->CameraUpdate(mainCamera_);
	blocks_->Draw();

	//door
	doors_->CameraUpdate(mainCamera_);
	doors_->Draw();

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	fade_->Draw();

}

void GameScene::Finalize()
{

}

void GameScene::CreateLevel()
{
	// マップチップフィールドの生成
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	blocks_ = std::make_unique<InstancingObjects>();
	blocks_->Initialize(modelBlock_.get(), mapChipField_->GetNumCellVirtical() * mapChipField_->GetNumCellHorizontal());
	blocks_->PreUpdate();

	doors_ = std::make_unique<InstancingObjects>();
	doors_->Initialize(modelBlock_.get(), mapChipField_->GetNumCellVirtical() * mapChipField_->GetNumCellHorizontal());
	doors_->PreUpdate();

	WorldTransform worldTransform = {};

	for (uint32_t y = 0; y < mapChipField_->GetNumCellVirtical(); y++) {
		for (uint32_t x = 0; x < mapChipField_->GetNumCellHorizontal(); x++) {
			if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kBlock) 
			{
				worldTransform.Initialize();
				worldTransform.translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
				worldTransform.UpdateMatrix();
				blocks_->WorldTransformUpdate(worldTransform);
			}
			else if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kDoor)
			{
				worldTransform.Initialize();
				worldTransform.translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
				worldTransform.UpdateMatrix();
				doors_->WorldTransformUpdate(worldTransform);
			}
		}
	}
}

void GameScene::UpdateStart()
{
	fade_->Update();
	if (fade_->IsFinished())
	{
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void GameScene::UpdateMain()
{
	//プレイヤーの更新
	player_->Update();

	cameraController_->Update();
}

void GameScene::UpdateGameClear()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		//fade_->Stop();
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameClearScene");
	}
}

void GameScene::UpdateGameOver()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		//fade_->Stop();
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameOverScene");
	}
}
void GameScene::CheckElectricCollision() {
	if (!player_ || !blocks_) return;

	auto electricRange = player_->GetElectricRange();
	if (!electricRange) return;

	Vector3 electricPos = electricRange->GetPosition();      //電気の中心
	float electricRadius = electricRange->GetScale().x * 0.5f; //電気の半径

	for (uint32_t i = 0; i < blocks_->GetNumInstance(); ++i) {
		Vector3 blockPos = blocks_->GetInstancePosition(i);
		float blockRadius = 1.1999999999999f; //ブロック1マスの半径

		Vector3 diff = electricPos - blockPos;
		float dist = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

		if (dist <= electricRadius + blockRadius) {
			std::string msg = "Electric hit block " + std::to_string(i) + "\n";
			OutputDebugStringA(msg.c_str()); printf("Electric hit block %u\n", i);
		}
	}
}
