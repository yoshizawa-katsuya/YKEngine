#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "RigidModel.h"
#include "Rigid3dObject.h"
#include <numbers>
#include "Collision.h"

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
	camera_->SetRotateX(0.741f);
	camera_->SetRotateZ(0.0f);
	camera_->SetTranslate({ 0.0f, 31.31f, -33.19f });

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
	
	modelstone_ = std::make_unique<RigidModel>();
	modelstone_->CreateModel("./resources/stone", "stone.obj");
	
	modelstar_ = std::make_unique<RigidModel>();
	modelstar_->CreateModel("./resources/star", "star.obj");

	modelhole_ = std::make_unique<RigidModel>();
	modelhole_->CreateModel("./resources/hole", "hole.obj");

	modelice_ = std::make_unique<RigidModel>();
	modelice_->CreateModel("./resources/ice", "ice.obj");

	/*
	//テクスチャハンドルの生成
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/player/Player.png");

	//スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	*/
	//background
	background_ = TextureManager::GetInstance()->Load("./resources/scene/background1.png");
	backgroundSprite_ = std::make_unique<Sprite>();
	backgroundSprite_->Initialize(background_);
	//backgroundSprite_->SetPosition({ 0.0f,0.0f });


	//マップチップフィールドの生成
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("./resources/csv/stage1.csv");

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());

	
	GenerateObjects();


	SceneData data = SceneManager::GetInstance()->GetSceneData();

	selectedTutorial_ = data.selectedTutorial;
	selectedBundle_ = data.selectedBundle;
	selectedStage_ = data.selectedStage;

	
}

void GameScene::Update() {
	input_->Update();
	

	//カメラの更新
	camera_->Update();

	

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}
	//stone move

	Vector2 mousePos = input_->GetMousePosition();

	if (input_->PushMouseLeft()) {
		Vector3 clickPos = ConvertScreenToWorld(mousePos);

		float distance = static_cast<float>(sqrt(pow(clickPos.x - stonePosition_.x, 2) + pow(clickPos.z - stonePosition_.z, 2)));

		if (distance < 1.0f) { 
			isDragging_ = true;
			dragStartPos_ = mousePos;
		}
	}
	if (isDragging_ && input_->PushMouseLeft()) {
		dragCurrentPos_ = input_->GetMousePosition();
	}
	if (isDragging_ && !input_->PushMouseLeft() && !input_->TrigerMouseLeft()) {
		isDragging_ = false;
		Vector2 dragVector = { dragStartPos_.x - dragCurrentPos_.x  , dragStartPos_.y - dragCurrentPos_.y };

		float length = sqrt(dragVector.x * dragVector.x + dragVector.y * dragVector.y);
		if (length > 0) {
			dragVector.x /= length;
			dragVector.y /= length;
		}

		float speed = std::min(length * 0.03f, maxSpeed_);  
		velocity_ = { dragVector.x * speed, 0.0f, -dragVector.y * speed };  

	}


	stonePosition_ += velocity_;
	velocity_ *= friction_;

	if (fabs(velocity_.x) < 0.01f) velocity_.x = 0.0f;
	if (fabs(velocity_.z) < 0.01f) velocity_.z = 0.0f;

	WorldTransform worldTransform;
	worldTransform.Initialize();
	worldTransform.translation_ = stonePosition_;
	worldTransform.UpdateMatrix();
	stone_->WorldTransformUpdate(worldTransform);

	
	//player_->Update();


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
		

		ImGui::Begin("GameScene Debug");


			ImGui::Text("Selected Tutorial: %u", selectedTutorial_);
			ImGui::Text("Selected Bundle: %u", selectedBundle_);
			ImGui::Text("Selected Stage: %u", selectedStage_);
			ImGui::Text("stonePosition_x: %f", stonePosition_.x);
			ImGui::Text("stonePosition_y: %f", stonePosition_.y);
			ImGui::Text("dragStartPos＿: %f %f", dragStartPos_.x, dragStartPos_.y);
			ImGui::Text("dragCurrentPos＿: %f %f", dragCurrentPos_.x, dragCurrentPos_.y);
			
		ImGui::End();

#endif // _DEBUG
	

}

void GameScene::Draw() {

	//Spriteの背景描画前処理
	spritePlatform_->PreBackGroundDraw();

	backgroundSprite_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//modelPlatform_->SkinPreDraw();

	//プレイヤーの描画
	//player_->Draw(mainCamera_);

	//boxの描画
	for (auto& boxesLine : boxes_) {
		for (auto& box : boxesLine) {
			if (box) {
				box->CameraUpdate(mainCamera_);
				box->Draw();
			}
		}
	}
	stone_->CameraUpdate(mainCamera_);
	stone_->Draw();

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
			Vector3 position = mapChipField_->GetMapChipPositionByIndex(j, i);
			Vector3 defaultScale = { 1.0f, 1.0f, 1.0f };
			if (mapChipType == MapChipType::kBox) {
				CreateObject(boxes_[i][j], modelBox_.get(), position, defaultScale);
			} else if (mapChipType == MapChipType::kFloor) {
				AddToInstancing(floors_.get(), position);
			}  else if (mapChipType == MapChipType::stone) {
				stone_ = std::make_unique<Rigid3dObject>();
				stone_->Initialize(modelstone_.get());
				stonePosition_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			} else if (mapChipType == MapChipType::star) {
				CreateObject(boxes_[i][j], modelstar_.get(), position, defaultScale);
			} else if (mapChipType == MapChipType::hole) {
				CreateObject(boxes_[i][j], modelhole_.get(), position, defaultScale);
			} else if (mapChipType == MapChipType::ice) {
				CreateObject(boxes_[i][j], modelice_.get(), position, { 0.5f, 0.5f, 0.5f });
			}
		}
	}


}



void GameScene::CreateObject(std::unique_ptr<Base3dObject>& object, BaseModel* model, const Vector3& position, const Vector3& scale)
{
	object = std::make_unique<Rigid3dObject>(); 
	object->Initialize(model); 
	WorldTransform* worldTransform = new WorldTransform();
	worldTransform->Initialize();
	worldTransform->translation_ = position; 
	worldTransform->scale_ = scale;
	worldTransform->UpdateMatrix();
	object->WorldTransformUpdate(*worldTransform);
}

void GameScene::AddToInstancing(InstancingObject* instancingObject, const Vector3& position)
{
	WorldTransform* worldTransform = new WorldTransform();
	worldTransform->Initialize();
	worldTransform->translation_ = position;
	worldTransform->UpdateMatrix();
	instancingObject->AddWorldTransform(*worldTransform);

}

Vector3 GameScene::ConvertScreenToWorld(const Vector2& screenPos)
{
	float worldX = (screenPos.x / 990.0f) * 34.0f - 17.0f; 
	float worldZ = (screenPos.y / 600.0f) * 30.0f - 15.0f; 

	return Vector3(worldX, 0.0f, -worldZ);
}
