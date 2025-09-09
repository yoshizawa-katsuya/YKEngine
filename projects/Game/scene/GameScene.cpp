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
	textureHandle2_ = TextureManager::GetInstance()->Load("./resources/rostock_laage_airport_4k.dds");
	uint32_t textureHandleGoal = TextureManager::GetInstance()->Load("./resources/goal.png");

	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateRigidModel("./resources/Player", "Player.obj");
	modelBlock_ = modelPlatform_->CreateRigidModel("./resources/block", "block.obj");
	modelSpine_ = modelPlatform_->CreateRigidModel("./resources/spine", "spine.obj");
	modelGoal_ = modelPlatform_->CreateRing(textureHandleGoal, "goal");
	modelGoal_->SetEnableLighting(false);
	modelElectric_ = modelPlatform_->CreateRigidModel("./resources/Denki", "denkih.obj");

	CreateLevel();
	
	//カメラコントローラーの生成
	cameraController_ = std::make_unique<CameraController>();
	cameraController_->Initialize(camera_.get(), player_.get(), mapChipField_.get());

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);

	doorGimmick_ = std::make_unique<Door>();
	doorGimmick_->Initialize(mapChipField_.get());

	spineGimmick_ = std::make_unique<SpineGimmick>();
	spineGimmick_->Initialize(mapChipField_.get());
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

	goal_->Draw(mainCamera_);

	//スカイボックスの描画前処理
	//modelPlatform_->SkyBoxPreDraw();

	//modelPlatform_->SkinPreDraw();

	modelPlatform_->InstancingPreDraw();

	blocks_->CameraUpdate(mainCamera_);
	blocks_->Draw();

	trapSpines_->CameraUpdate(mainCamera_);
	trapSpines_->Draw();

	//door
	if (doorGimmick_->GetState() == Door::State::kClosed) {
		doors_->CameraUpdate(mainCamera_);
		doors_->Draw();
	}

	if (spineGimmick_->GetState() == SpineGimmick::State::kActive) {
		gimmickSpines_->CameraUpdate(mainCamera_);
		gimmickSpines_->Draw();
	}

	doorTriggers_->CameraUpdate(mainCamera_);
	doorTriggers_->Draw();

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
	mapChipField_->LoadMapChipCsv("Resources/stageData/stage" + std::to_string(stageNum_) + ".csv");

	//マップの生成
	blocks_ = std::make_unique<InstancingObjects>();
	blocks_->Initialize(modelBlock_.get(), mapChipField_->GetNumBlocks());
	blocks_->PreUpdate();

	doors_ = std::make_unique<InstancingObjects>();
	doors_->Initialize(modelBlock_.get(), mapChipField_->GetNumCellVirtical() * mapChipField_->GetNumCellHorizontal());
	doors_->PreUpdate();

	doorTriggers_ = std::make_unique<InstancingObjects>();
	doorTriggers_->Initialize(modelBlock_.get(), mapChipField_->GetNumCellVirtical() * mapChipField_->GetNumCellHorizontal());
	doorTriggers_->PreUpdate();

	trapSpines_ = std::make_unique<InstancingObjects>();
	trapSpines_->Initialize(modelSpine_.get(), mapChipField_->GetNumSpines());
	trapSpines_->PreUpdate();

	spineTriggers_ = std::make_unique<InstancingObjects>();
	spineTriggers_->Initialize(modelBlock_.get(), mapChipField_->GetNumCellVirtical() * mapChipField_->GetNumCellHorizontal());
	spineTriggers_->PreUpdate();

	gimmickSpines_ = std::make_unique<InstancingObjects>();
	gimmickSpines_->Initialize(modelSpine_.get(), mapChipField_->GetNumSpines());
	gimmickSpines_->PreUpdate();

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
				goal_->SetPosition(mapChipField->GetMapChipPositionByIndex(x, y));

				break;

			case MapChipType::kDoorTrigger:

				setWorldTransform(x, y);
				doorTriggers_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kClosedDoor:

				setWorldTransform(x, y);
				doors_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kActiveSpine:
					
			    setWorldTransform(x, y);
			    gimmickSpines_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kSpineTrigger:

				setWorldTransform(x, y);
				spineTriggers_->WorldTransformUpdate(worldTransform);

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
	//プレイヤーの更新
	player_->Update();

	cameraController_->Update();

	if (CheckElectricCollision(MapChipType::kDoorTrigger)) {

		doorGimmick_->ChangeState(Door::State::kOpened);
	}

	if (CheckElectricCollision(MapChipType::kSpineTrigger))
	{
		spineGimmick_->ChangeState(SpineGimmick::State::kInactive);
	}

	goal_->Update();

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
	if (!player_) return false;

	auto electricRange = player_->GetElectricRange();
	if (!electricRange) return false;

	Vector3 electricPos = electricRange->GetPosition();      //電気の中心
	float electricRadius = electricRange->GetScale().x; //電気の半径

	switch (type)
	{
	case MapChipType::kDoorTrigger:

		for (uint32_t i = 0; i < doorTriggers_->GetNumInstance(); ++i) {
			Vector3 blockPos = doorTriggers_->GetInstancePosition(i);

			float half = 2.0f / 2.0f;

			// AABB の最小・最大
			Vector3 min = { blockPos.x - half, blockPos.y - half, 0.0f };
			Vector3 max = { blockPos.x + half, blockPos.y + half, 0.0f };

			// 円の中心に一番近い点をAABBの範囲にクランプ
			Vector3 closest = {
				std::clamp(electricPos.x, min.x, max.x),
				std::clamp(electricPos.y, min.y, max.y),
				0.0f
			};

			// 最近接点と円の中心の距離を比較
			Vector3 diff = electricPos - closest;
			float distSq = diff.x * diff.x + diff.y * diff.y;

			if (distSq <= (electricRadius * electricRadius)) {

				return true;
			}
		}

		break;

	case MapChipType::kSpineTrigger:

		for (uint32_t i = 0; i < spineTriggers_->GetNumInstance(); ++i) {
			Vector3 blockPos = spineTriggers_->GetInstancePosition(i);

			float half = 2.0f / 2.0f;

			// AABB の最小・最大
			Vector3 min = { blockPos.x - half, blockPos.y - half, 0.0f };
			Vector3 max = { blockPos.x + half, blockPos.y + half, 0.0f };

			// 円の中心に一番近い点をAABBの範囲にクランプ
			Vector3 closest = {
				std::clamp(electricPos.x, min.x, max.x),
				std::clamp(electricPos.y, min.y, max.y),
				0.0f
			};

			// 最近接点と円の中心の距離を比較
			Vector3 diff = electricPos - closest;
			float distSq = diff.x * diff.x + diff.y * diff.y;

			if (distSq <= (electricRadius * electricRadius)) {

				return true;
			}
		}

		break;
	}

	return false;
}
