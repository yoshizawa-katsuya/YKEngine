#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "RigidModel.h"
#include "SkinModel.h"

GameScene::~GameScene() {
	//Finalize();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize(dxCommon_);

	modelPlatform_->SetDirectionalLight(directionalLight_.get());

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

	textureHandle_ = TextureManager::GetInstance()->Load("./resources/white.png");

	// プレイヤーのモデル部位ごとの読み込み
	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/Body", "Body.obj");

	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/Head", "Head.obj");

	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/RightArm", "RightArm.obj");

	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/LeftArm", "LeftArm.obj");

	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/RightLeg", "RightLeg.obj");

	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/LeftLeg", "LeftLeg.obj");

	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/Booster", "Booster.obj");

	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/Jet", "Jet.obj");

	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/Gun", "Gun.obj");

	playerModels_.emplace_back(std::make_unique<RigidModel>());
	playerModels_.back()->CreateModel("./resources/player/MissileLauncher", "MissileLauncher.obj");


	

	 //Bossモデルの生成
	 bossModels_.emplace_back(std::make_unique<RigidModel>());
	 bossModels_.back()->CreateModel("./resources/Boss/Connection", "Connection.obj");//0

	 bossModels_.emplace_back(std::make_unique<RigidModel>());
	 bossModels_.back()->CreateModel("./resources/Boss/BossBody", "BossBody.obj");//1

	 bossModels_.emplace_back(std::make_unique<RigidModel>());
	 bossModels_.back()->CreateModel("./resources/Boss/BossHead", "BossHead.obj");//2

	 bossModels_.emplace_back(std::make_unique<RigidModel>());
	 bossModels_.back()->CreateModel("./resources/Boss/BossGunR", "BossGunR.obj");//3

	 bossModels_.emplace_back(std::make_unique<RigidModel>());
	 bossModels_.back()->CreateModel("./resources/Boss/BossGunL", "BossGunL.obj");//4

	 bossModels_.emplace_back(std::make_unique<RigidModel>());
	 bossModels_.back()->CreateModel("./resources/Boss/TrackL", "TrackL.obj");//5

	 bossModels_.emplace_back(std::make_unique<RigidModel>());
	 bossModels_.back()->CreateModel("./resources/Boss/TrackR", "TrackR.obj");//6


	/*
	//テクスチャハンドルの生成
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/player/Player.png");

	//スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	*/

	// プレイヤーの初期化
	 std::vector<BaseModel*> modelPtrs;
	 for (auto& model : playerModels_) {
		 modelPtrs.push_back(model.get());
	 }
	 player_ = std::make_unique<Player>();
	 player_->Initialize(modelPtrs);

	//Bossの初期化
	 std::vector<BaseModel*> bossModelPtrs;
	 for (auto& model : bossModels_) {
		 bossModelPtrs.push_back(model.get());
	 }
	 boss_ = std::make_unique<Boss>();
	 boss_->Initialize(bossModelPtrs);


	

}

void GameScene::Update() {

	input_->GamePadUpdate();

	/*
	input_->PushButton(XINPUT_GAMEPAD_A);
	input_->TriggerButton(XINPUT_GAMEPAD_B);
	input_->ReleaseButton(XINPUT_GAMEPAD_X);
	input_->HoldButton(XINPUT_GAMEPAD_Y);
	input_->GetLeftStickX();
	*/

	//カメラの更新
	camera_->Update();

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	//プレイヤーの更新
	player_->Update();


	boss_->Update();

#ifdef _DEBUG


		ImGui::Begin("Window");
		if (ImGui::TreeNode("camera")) {
			ImGui::DragFloat3("translate", &camera_->GetTranslate().x, 0.01f);
			ImGui::DragFloat3("rotate", &camera_->GetRotate().x, 0.01f);
			//ImGui::DragFloat3("scale", &cameratransform.scale.x, 0.01f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("directionalLight")) {
			ImGui::ColorEdit4("color", &directionalLight_->GetColor().x);
			ImGui::DragFloat3("direction", &directionalLight_->GetDirection().x, 0.01f);
			ImGui::DragFloat("intensity", &directionalLight_->GetIntensity(), 0.01f);

			ImGui::TreePop();
		}
		//メインカメラの切り替え
		if (ImGui::RadioButton("gameCamera", !isActiveDebugCamera_)) {
			isActiveDebugCamera_ = false;

			mainCamera_ = camera_.get();


		}
		if (ImGui::RadioButton("DebugCamera", isActiveDebugCamera_)) {
			isActiveDebugCamera_ = true;

			mainCamera_ = camera2_.get();

		}
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
	spritePlatform_->PreBackGroundDraw();

	//sprite_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//modelPlatform_->SkinPreDraw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	boss_->Draw(mainCamera_);



	//Spriteの描画前処理
	spritePlatform_->PreDraw();




}

void GameScene::Finalize()
{

}