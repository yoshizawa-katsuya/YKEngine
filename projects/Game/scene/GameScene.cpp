#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "RigidModel.h"

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
	directionalLight_->Initialize(dxCommon_);

	//点光源の生成
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize(dxCommon_);

	//スポットライトの生成
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize(dxCommon_);

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

	//モデルを描画する際ライトとカメラの設定は必須
	modelPlatform_->SetDirectionalLight(directionalLight_.get());
	modelPlatform_->SetPointLight(pointLight_.get());
	modelPlatform_->SetCamera(mainCamera_);
	modelPlatform_->SetSpotLight(spotLight_.get());

	textureHandle_ = TextureManager::GetInstance()->Load("./resources/circle.png");

	//モデルの生成
	modelPlayer_ = std::make_unique<RigidModel>();
	modelPlayer_->CreateModel("./resources/Player", "Player.obj");
	//modelPlayer_->CreateSphere(textureHandle_);
	
	/*
	//テクスチャハンドルの生成
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/player/Player.png");

	//スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	*/

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());


	//select Sprite
	easySprite_ = TextureManager::GetInstance()->Load("./resources/selectStageEasy.png");
	normalSprite_ = TextureManager::GetInstance()->Load("./resources/selectStageNormal.png");
	hardSprite_ = TextureManager::GetInstance()->Load("./resources/selectStageHard.png");

	selectEasySprite_ = std::make_unique<Sprite>();
	selectEasySprite_->Initialize(easySprite_);
	selectEasySprite_->SetPosition({ 100.0f, 50.0f });

	selectNormalSprite_ = std::make_unique<Sprite>();
	selectNormalSprite_->Initialize(normalSprite_);
	selectNormalSprite_->SetPosition({ 200.0f, 50.0f });

	selectHardSprite_ = std::make_unique<Sprite>();
	selectHardSprite_->Initialize(hardSprite_);
	selectHardSprite_->SetPosition({ 300.0f, 50.0f });
	
	// マップチップフィールドの生成
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("Resources/stage/stage1.csv");
	//map chip model
	modelBlock_ = std::make_unique<RigidModel>();
	modelBlock_->CreateModel("./resources/box", "box.obj");

}

void GameScene::Update() {

	

	//カメラの更新
	camera_->Update();

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	//プレイヤーの更新
	player_->Update();

	//emitter_->Update(color_);

	//ParticleManager::GetInstance()->Update(mainCamera_, field_.get());

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

	//sprite_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//modelPlatform_->SkinPreDraw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);


	//ブロックの描画
	for (std::vector<std::unique_ptr<WorldTransform>>& worldTransformBlockLine : worldTransformBlocks_) {
		for (std::unique_ptr<WorldTransform>& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw();
		}
	}

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	selectEasySprite_->Draw();
	selectNormalSprite_->Draw();
	selectHardSprite_->Draw();

	//ParticleManager::GetInstance()->Draw();
}

void GameScene::Finalize()
{

}

void GameScene::GeneratrBlocks()
{
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 配列を初期化
	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// キューブと床の生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			// ブロックの生成
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				worldTransformBlocks_[i][j] = std::make_unique<WorldTransform>();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}

		}
	}

}
