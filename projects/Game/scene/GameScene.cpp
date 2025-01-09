#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "RigidModel.h"
#include "Rigid3dObject.h"
#include <numbers>

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
	camera_->SetRotateX(std::numbers::pi_v<float> / 2.0f);
	camera_->SetTranslate({ 0.0f, 10.0f, 0.0f });

	//デバッグカメラの生成
	camera2_ = std::make_unique<Camera>();
	camera2_->SetTranslate({ 0.0f, 10.0f, 0.0f });
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(camera2_.get(), input_);
	debugCamera_->AddRotateX(std::numbers::pi_v<float> / 2.0f);

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
	
	modelBox_ = std::make_unique<RigidModel>();
	modelBox_->CreateModel("./resources/box", "box.obj");
	
	modelFloor_ = std::make_unique<RigidModel>();
	modelFloor_->CreateModel("./resources/floor", "Floor.obj");
	/*
	//テクスチャハンドルの生成
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/player/Player.png");

	//スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	*/

	//マップチップフィールドの生成
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("./resources/csv/stage1.csv");

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());

	GenerateObjects();
	
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

	//boxの描画
	for (std::vector<std::unique_ptr<Base3dObject>>& boxesLine : boxes_) {
		for (std::unique_ptr<Base3dObject>& box : boxesLine) {
			if (!box) {
				continue;
			}
			box->CameraUpdate(mainCamera_);
			box->Draw();
		}
	}

	//instancingObject描画前処理
	modelPlatform_->InstancingPreDraw();

	//floorの描画
	floors_->CameraUpdate(mainCamera_);
	floors_->Draw();
	
	//Spriteの描画前処理
	//spritePlatform_->PreDraw();

	//ParticleManager::GetInstance()->Draw();

}

void GameScene::Finalize()
{

}

void GameScene::GenerateObjects()
{

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	//要素数を変更する
	boxes_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		boxes_[i].resize(numBlockHorizontal);
	}

	floors_ = std::make_unique<InstancingObject>();
	floors_->Initialize(modelFloor_.get(), numBlockVirtical * numBlockHorizontal);

	MapChipType mapChipType;

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			mapChipType = mapChipField_->GetMapChipTypeByIndex(j, i);
			if (mapChipType == MapChipType::kBox) {
				Base3dObject* objectBox = new Rigid3dObject;
				objectBox->Initialize(modelBox_.get());
				boxes_[i][j].reset(objectBox);
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				worldTransform->UpdateMatrix();
				boxes_[i][j]->WorldTransformUpdate(*worldTransform);
			}
			else if (mapChipType == MapChipType::kFloor) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				worldTransform->UpdateMatrix();
				floors_->AddWorldTransform(*worldTransform);
			}
		}
	}


}
