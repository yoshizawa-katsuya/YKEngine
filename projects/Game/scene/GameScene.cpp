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
	//modelPlatform_->SetDirectionalLight(directionalLight_.get());
	//modelPlatform_->SetPointLight(pointLight_.get());
	modelPlatform_->SetCamera(mainCamera_);
	//modelPlatform_->SetSpotLight(spotLight_.get());

	//textureHandle_ = TextureManager::GetInstance()->Load("./resources/circle.png");
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/white.png");
	uint32_t textureHandle2 = TextureManager::GetInstance()->Load("./resources/circle2.png");
	uint32_t textureHandle3 = TextureManager::GetInstance()->Load("./resources/ring.png");

	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateRigidModel("./resources/Player", "Player.obj");
	//modelPlayer_->SetUVTransform({ 10.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	//modelPlayer_->SetEnableLighting(false);
	std::shared_ptr<BaseModel> ringModel_ = modelPlatform_->CreateRing(textureHandle3);
	ringModel_->SetUVTransform({ 20.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

	/*
	//スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	*/

	//パーティクルエミッターの生成
	slashEmitter_ = std::make_unique<ParticleEmitter>("SlashEffect", 5, 1.5f);
	slashEmitter_->Initialize(textureHandle2, modelPlatform_->CreatePlane(textureHandle2));
	slashEmitter_->SetScale({ 0.2f, 1.0f, 1.0f });
	slashEmitter_->SetIsRandomRotate(true);
	slashEmitter_->SetRandScaleMax({ 0.0f, 3.0f, 0.0f });
	slashEmitter_->SetRandScaleMin({ 0.0f, 0.0f, 0.0f });
	slashEmitter_->SetIsRandomScale(true);
	slashEmitter_->SetRandRotateMax({ 0.0f, 0.0f, std::numbers::pi_v<float> });
	slashEmitter_->SetRandRotateMin({ 0.0f, 0.0f, -std::numbers::pi_v<float> });
	slashEmitter_->SetIsRandomLifeTime(true);
	slashEmitter_->SetRandLifeTimeMax(0.2f);
	slashEmitter_->SetRandLifeTimeMin(0.2f);
	slashEmitter_->SetIsScaleToAppear(true);
	slashEmitter_->SetEasingTypeForScale(EasingType::EaseOutCubic);
	//slashEmitter_->SetIsScaleToDisappear(true);
	//slashEmitter_->SetEasingTypeForScale(EasingType::EaseInBack);

	slashEmitter2_ = std::make_unique<ParticleEmitter>("SlashEffect2", 5, 1.5f);
	slashEmitter2_->Initialize(textureHandle2, modelPlatform_->CreatePlane(textureHandle2));
	slashEmitter2_->SetScale({ 0.2f, 1.0f, 1.0f });
	slashEmitter2_->SetIsRandomRotate(true);
	slashEmitter2_->SetRandScaleMax({ 0.0f, 3.0f, 0.0f });
	slashEmitter2_->SetRandScaleMin({ 0.0f, 0.0f, 0.0f });
	slashEmitter2_->SetIsRandomScale(true);
	slashEmitter2_->SetRandRotateMax({ 0.0f, 0.0f, std::numbers::pi_v<float> });
	slashEmitter2_->SetRandRotateMin({ 0.0f, 0.0f, -std::numbers::pi_v<float> });
	slashEmitter2_->SetIsRandomLifeTime(true);
	slashEmitter2_->SetRandLifeTimeMax(0.2f);
	slashEmitter2_->SetRandLifeTimeMin(0.2f);
	slashEmitter2_->SetIsScaleToDisappear(true);
	slashEmitter2_->SetEasingTypeForScale(EasingType::EaseInBack);

	explosionEmitter_ = std::make_unique<ParticleEmitter>("ExplosionEffect", 30, 1.5f);
	explosionEmitter_->Initialize(textureHandle_, modelPlayer_);
	explosionEmitter_->SetIsFaceToVelocityDirection(true);
	explosionEmitter_->SetIsRandomVelocity(true);
	explosionEmitter_->SetRandVelocityMax({ 1.0f, 1.0f, 1.0f });
	explosionEmitter_->SetRandVelocityMin({ -1.0f, -1.0f, -1.0f });
	explosionEmitter_->SetScale({ 0.05f, 0.05f, 1.5f });
	explosionEmitter_->SetIsConstantVelocity(true);
	explosionEmitter_->SetSpeed(20.0f);
	explosionEmitter_->SetIsRandomLifeTime(true);
	explosionEmitter_->SetRandLifeTimeMax(0.2f);
	explosionEmitter_->SetRandLifeTimeMin(0.2f);
	explosionEmitter_->SetIsScaleToDisappear(true);
	explosionEmitter_->SetIsDownVelocity(true);

	ringEmitter_ = std::make_unique<ParticleEmitter>("RingEffect", 1, 1.5f);
	ringEmitter_->Initialize(textureHandle3, ringModel_);
	ringEmitter_->SetIsRandomLifeTime(true);
	ringEmitter_->SetRandLifeTimeMax(0.2f);
	ringEmitter_->SetRandLifeTimeMin(0.2f);
	ringEmitter_->SetIsScaleToAppear(true);
	ringEmitter_->SetEasingTypeForScale(EasingType::EaseOutCubic);

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());

	/*
	objects_ = std::make_unique<InstancingObjects>();
	objects_->Initialize(modelPlayer_.get(), 10);

	worldTransform1_.Initialize();
	worldTransform1_.translation_.x = -1.0f;
	worldTransform1_.UpdateMatrix();

	worldTransform2_.Initialize();
	worldTransform2_.translation_.x = 1.0f;
	worldTransform2_.UpdateMatrix();
	*/
}

void GameScene::Update() {



	//カメラの更新
	camera_->Update();

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	//プレイヤーの更新
	player_->Update();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_->GetDirectionalLightData());
	//modelPlatform_->PointLightUpdate(pointLight_->GetPointLightData());
	//modelPlatform_->SpotLightUpdate(spotLight_->GetSpotLightData());

	/*
	objects_->PreUpdate();
	worldTransform1_.translation_.x += 0.01f;
	worldTransform1_.UpdateMatrix();
	objects_->WorldTransformUpdate(worldTransform1_);
	objects_->WorldTransformUpdate(worldTransform2_);
	*/

	/*slashEmitter_->Update();
	explosionEmitter_->Update({1.0f, 0.0f, 0.0f, 1.0f});*/
	if (isParticleUpdate_)
	{
		ParticleManager::GetInstance()->Update(mainCamera_);
	}
	//if (input_->TriggerKey(DIK_SPACE)) {
	//	//シーン切り替え依頼
	//	sceneManager_->ChengeScene("TitleScene");
	//}

#ifdef _DEBUG

	
	ImGui::Begin("ParticleDebug");

	ImGui::Checkbox("isParticleUpdate", &isParticleUpdate_);

	if (ImGui::Button("advance1Frame"))
	{
		ParticleManager::GetInstance()->Update(mainCamera_);
	}
	if (ImGui::TreeNode("emitter"))
	{
		if (ImGui::Button("Slash1"))
		{
			slashEmitter_->Emit({ 1.0f, 0.5f, 0.0f, 1.0f });
		}
		if (ImGui::Button("Slash2"))
		{
			slashEmitter2_->Emit({ 1.0f, 0.5f, 0.0f, 1.0f });
		}
		if (ImGui::Button("Explosion")) 
		{
			explosionEmitter_->Emit({ 1.0f, 1.0f, 0.0f, 1.0f });
		}
		if (ImGui::Button("Ring")) 
		{
			ringEmitter_->Emit({ 1.0f, 0.2f, 0.0f, 1.0f });
		}
		if (ImGui::Button("Group1")) 
		{
			//パーティクルの発生
			slashEmitter_->Emit({ 1.0f, 0.5f, 0.0f, 1.0f });
			explosionEmitter_->Emit({ 1.0f, 1.0f, 0.0f, 1.0f });
			ringEmitter_->Emit({ 1.0f, 0.2f, 0.0f, 1.0f });
		}
		if (ImGui::Button("Group2"))
		{
			//パーティクルの発生
			slashEmitter2_->Emit({ 1.0f, 0.5f, 0.0f, 1.0f });
			explosionEmitter_->Emit({ 1.0f, 1.0f, 0.0f, 1.0f });
			ringEmitter_->Emit({ 1.0f, 0.2f, 0.0f, 1.0f });
		}

		ImGui::TreePop();
	}

	ImGui::End();


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
	//player_->Draw(mainCamera_);

	

	/*
	modelPlatform_->InstancingPreDraw();

	objects_->CameraUpdate(mainCamera_);
	objects_->Draw();
	*/
	//Spriteの描画前処理
	//spritePlatform_->PreDraw();

	ParticleManager::GetInstance()->Draw();

}

void GameScene::Finalize()
{

}