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

	emitter_ = std::make_unique<ParticleEmitter>("bord", 10, 0.5f);
	emitter_->Initialize(textureHandle_);
	emitter_->SetIsRandomColor(true);
	//emitter_->SetScale({ 0.5f, 0.5f, 0.5f });
	//EulerTransform trnaform{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
	//emitter_->SetTransform(trnaform);

	field_ = std::make_unique<AccelerationField>();
	field_->accerelation = { 5.0f, 0.0f, 0.0f };
	field_->area.min = { -1.0f, -1.0f, -1.0f };
	field_->area.max = { 1.0f, 1.0f, 1.0f };
}

void GameScene::Update() {

	

	//カメラの更新
	camera_->Update();

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	//プレイヤーの更新
	player_->Update();

	//emitter_->Update({ 1.0f, 0.3f, 0.0f, 1.0f });
	emitter_->Update(color_);

	ParticleManager::GetInstance()->Update(mainCamera_, field_.get());

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
		
		/*
		if (ImGui::Button("BGMstop")) {
			audio_->SoundStopWave(bgm1_);
		}
		*/
		ImGui::End();

		ImGui::Begin("Particle");
		ImGui::ColorEdit4("color", &color_.x);
		ImGui::DragFloat3("Translate", &emitter_->GetTranslate().x, 0.01f);
		ImGui::DragFloat3("Scele", &emitter_->GetScele().x, 0.01f);
		ImGui::DragFloat3("TranslateMin", &emitter_->GetRandTranslateMin().x, 0.01f);
		ImGui::DragFloat3("TranslateMax", &emitter_->GetRandTranslateMax().x, 0.01f);
		int count = emitter_->GetCount();
		ImGui::DragInt("Count", &count, 0.1f);
		if (count < 0) {
			count = 0;
		}
		emitter_->SetCount(count);
		ImGui::DragFloat("Frequency", &emitter_->GetFrequency(), 0.01f);
		ImGui::Checkbox("IsRandomColor", &emitter_->GetIsRandomColor());
		ImGui::DragFloat3("Accerelation", &field_->accerelation.x, 0.01f);
		ImGui::DragFloat3("FieldMin", &field_->area.min.x, 0.01f);
		ImGui::DragFloat3("FieldMax", &field_->area.max.x, 0.01f);
		if (ImGui::Button("Reset")) {
			color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
			emitter_->SetTranslation({ 0.0f, 0.0f, 0.0f });
			emitter_->SetRandTranslationMin({ -1.0f, -1.0f, -1.0f });
			emitter_->SetRandTranslationMax({ 1.0f, 1.0f, 1.0f });
			emitter_->SetCount(10);
			emitter_->SetFrequency(0.5f);
			emitter_->SetIsRandomColor(true);
			field_->accerelation = { 5.0f, 0.0f, 0.0f };
			field_->area.max = { 1.0f, 1.0f, 1.0f };
			field_->area.min = { -1.0f, -1.0f, -1.0f };
		}
		if (ImGui::Button("FireParticle")) {
			color_ = { 1.0f, 0.3f, 0.0f, 1.0f };
			emitter_->SetTranslation({ 0.0f, -2.0f, 0.0f });
			emitter_->SetRandTranslationMin({ -3.0f, 0.0f, -3.0f });
			emitter_->SetRandTranslationMax({ 3.0f, 0.3f, 1.0f });
			emitter_->SetCount(3);
			emitter_->SetFrequency(0.01f);
			emitter_->SetIsRandomColor(false);
			field_->accerelation = { 0.0f, 3.0f, 9.0f };
			field_->area.max = { 5.0f, 0.0f, 5.0f };
			field_->area.min = { -5.0f, -8.0f, -6.0f };
		}
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
	//player_->Draw(mainCamera_);

	//Spriteの描画前処理
	//spritePlatform_->PreDraw();

	ParticleManager::GetInstance()->Draw();

}

void GameScene::Finalize()
{

}