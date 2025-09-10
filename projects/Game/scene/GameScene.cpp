#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"

uint32_t GameScene::stageNum_ = 1;

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
	textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./resources/skyBox.dds");
	uint32_t textureHandleGoal = TextureManager::GetInstance()->Load("./resources/goal.png");

	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateRigidModel("./resources/Player", "Player.obj");
	modelBlock_ = modelPlatform_->CreateRigidModel("./resources/block", "block.obj");
	modelSpine_ = modelPlatform_->CreateRigidModel("./resources/spine", "spine.obj");
	modelGoal_ = modelPlatform_->CreateCylinder(textureHandleGoal, "goal");
	modelDischarge_ = modelPlatform_->CreateRing(textureHandleGoal, "goal");
	modelElectric_ = modelPlatform_->CreateRigidModel("./resources/Denki", "denki.obj");

	//スカイボックスの生成
	skyBox_ = std::make_unique<Rigid3dObject>();
	skyBox_->Initialize(modelPlatform_->CreateSkyBox(textureHandleSkyBox_).get());
	WorldTransform skyBoxTransform;
	skyBoxTransform.Initialize();
	skyBoxTransform.scale_ = { 100.0f, 100.0f, 100.0f };
	skyBoxTransform.UpdateMatrix();
	skyBox_->WorldTransformUpdate(skyBoxTransform);

	CreateLevel();

	//ポーズメニュー
	pause_ = std::make_unique<PauseMenu>();
	pause_->Initialize();

	gimmickManager_ = std::make_unique<GimmickManager>();
	gimmickManager_->Initialize(mapChipField_.get());
	player_->SetElectricModel(modelElectric_.get());

	//カメラコントローラーの生成
	cameraController_ = std::make_unique<CameraController>();
	cameraController_->Initialize(camera_.get(), player_.get(), mapChipField_.get());

	phase_ = Phase::kStart;

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);

	discharge_ = std::make_unique<Discharge>();
	discharge_->Initialize(modelDischarge_.get());
	dischargeVisible_ = false;
	dischargeFrame_ = 0;
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

	CheckElectricCollision(MapChipType::kDoorTrigger);

	cameraController_->Update();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_->GetDirectionalLightData());
	
	////リセット
	//if (input_->TriggerKey(DIK_R))
	//{
	//	Initialize();
	//}

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

	//環境マップを使う場合はコメントアウトを外す
	//TextureManager::GetInstance()->SetEnvironmentMap(textureHandleSkyBox_);

	//スカイボックスの描画前処理
	modelPlatform_->SkyBoxPreDraw();

	skyBox_->CameraUpdate(mainCamera_);
	skyBox_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	modelPlatform_->SetPipelineState(DrawMode::kCullBackMode);

	goal_->Draw(mainCamera_);

	modelPlatform_->SetPipelineState(DrawMode::kBlendModeNormal);
	//プレイヤーの描画
	player_->Draw(mainCamera_);

	

	//modelPlatform_->SkinPreDraw();

	modelPlatform_->InstancingPreDraw();

	blocks_->CameraUpdate(mainCamera_);
	blocks_->Draw();

	trapSpines_->CameraUpdate(mainCamera_);
	trapSpines_->Draw();

	gimmickManager_->Draw(mainCamera_);

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	if (dischargeVisible_) {
		discharge_->Draw(mainCamera_);
		modelPlatform_->SetPipelineState(DrawMode::kBlendModeNormal);   // 원복
	}

	fade_->Draw();
	//ポーズメニュー
	isDrawPauseUI = !(phase_ == Phase::kGameClear || phase_ == Phase::kGameOver);
	pause_->Draw(isDrawPauseUI);
}

void GameScene::Finalize()
{

}

void GameScene::CreateLevel()
{
	// マップチップフィールドの生成
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("Resources/stageData/stage" + std::to_string(stageNum_) + ".csv");

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->SetMapChipField(mapChipField_.get());
	player_->SetElectricModel(modelElectric_.get());

	//マップの生成
	blocks_ = std::make_unique<InstancingObjects>();
	blocks_->Initialize(modelBlock_.get(), mapChipField_->GetNumBlocks());
	blocks_->PreUpdate();

	trapSpines_ = std::make_unique<InstancingObjects>();

	trapSpines_->Initialize(modelSpine_.get(), mapChipField_->GetNumSpines());
	trapSpines_->PreUpdate();

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(modelGoal_.get());

	WorldTransform worldTransform = {};
	MapChipField* mapChipField = mapChipField_.get();
			
	std::function<void(uint32_t, uint32_t)> setWorldTransform = [&worldTransform, mapChipField](uint32_t x, uint32_t y) {
		worldTransform.Initialize();
		worldTransform.translation_ = mapChipField->GetMapChipPositionByIndex(x, y);
		worldTransform.UpdateMatrix();
		};

	for (uint32_t y = 0; y < mapChipField_->GetNumCellVirtical(); y++) {
		for (uint32_t x = 0; x < mapChipField_->GetNumCellHorizontal(); x++) {
			MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex(x, y);
			switch (mapChipType)
			{
			case MapChipType::kBlock:
				setWorldTransform(x, y);
				blocks_->WorldTransformUpdate(worldTransform);
				break;

			case MapChipType::kSpineTrap:
				setWorldTransform(x, y);
				trapSpines_->WorldTransformUpdate(worldTransform);
				break;

			case MapChipType::kGoal:
				goal_->SetPosition(mapChipField_->GetMapChipPositionByIndex(x, y));
				break;

				break;

			case MapChipType::kPlayerSpawn:

				//プレイヤーの初期化
				player_ = std::make_unique<Player>();
				Vector3 PlayerPosition = mapChipField_->GetMapChipPositionByIndex(x, y);
				player_->Initialize(modelPlayer_.get(), PlayerPosition);
				player_->SetMapChipField(mapChipField_.get());
				player_->SetElectricModel(modelElectric_.get());

				break;

			default:
				break;
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
	//ポーズ中は停止
	if (!pause_->IsPaused()) {
		//プレイヤーの更新
		player_->Update();

		cameraController_->Update();

	if (CheckElectricCollision(MapChipType::kDoorTrigger)) {

		gimmickManager_->DoorGimmickChangeState(Door::State::kOpened);
	}

	if (CheckElectricCollision(MapChipType::kSpineTrigger))
	{
		gimmickManager_->SpineGimmickChangeState(SpineGimmick::State::kInactive);
	}

	if (CheckElectricCollision(MapChipType::kDisappearTrigger))
	{
		gimmickManager_->DisappearGimmickChangeState(DisappearGimmick::State::kActive);
	}

	if (CheckElectricCollision(MapChipType::kAppearTrigger))
	{
		gimmickManager_->AppearGimmickChangeState(AppearGimmick::State::kActive);
	}
	

	goal_->Update();



	if (!dischargeVisible_) {
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			dischargeVisible_ = true;
			dischargeFrame_ = 0;
		}
	}

	if (dischargeVisible_) {
		if (auto er = player_->GetElectricRange()) {
			Vector3 pos = er->GetPosition();
			pos.z += 0.01f;
			discharge_->SetPosition(pos);
		}

		discharge_->Update();

		if (++dischargeFrame_ >= kDischargeLife) {
			dischargeVisible_ = false;   
		}
	}

		if (player_->HitGoal())
		{
			phase_ = Phase::kGameClear;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		}

		if (player_->GetIsDead())
		{
			phase_ = Phase::kGameOver;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		}
	}
	//ポーズメニュー
	pause_->Update();
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
bool GameScene::CheckElectricCollision(MapChipType type) {
	if (!player_ || !discharge_ || !dischargeVisible_) return false;

	Vector3 center = player_->GetElectricRange()
		? player_->GetElectricRange()->GetPosition()
		: Vector3{ 0,0,0 };

	
	float radius = discharge_->GetRadius();

	InstancingObjects* triggers_;

	switch (type)
	{
	case MapChipType::kDoorTrigger:

		triggers_ = gimmickManager_->GetTriggers(GimmickManager::GimmickType::kDoor);

		break;

	case MapChipType::kSpineTrigger:

		triggers_ = gimmickManager_->GetTriggers(GimmickManager::GimmickType::kSpine);

		break;

	case MapChipType::kDisappearTrigger:

		triggers_ = gimmickManager_->GetTriggers(GimmickManager::GimmickType::kDisappear);

		break;

	case MapChipType::kAppearTrigger:

		triggers_ = gimmickManager_->GetTriggers(GimmickManager::GimmickType::kAppear);

		break;

	default:

		return false;

	}

	const float half = 2.0f / 2.0f;

	for (uint32_t i = 0; i < triggers_->GetNumInstance(); ++i) {
		Vector3 p = triggers_->GetInstancePosition(i);

		Vector3 mn = { p.x - half, p.y - half, 0.0f };
		Vector3 mx = { p.x + half, p.y + half, 0.0f };

		Vector3 closest = {
			std::clamp(center.x, mn.x, mx.x),
			std::clamp(center.y, mn.y, mx.y),
			0.0f
		};

		Vector3 d = center - closest;
		float distSq = d.x * d.x + d.y * d.y;

		if (distSq <= radius * radius) {
			return true;
		}
	}
	return false;
}
