#include "GameScene.h"
#include "Vector.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "LevelDataLoader.h"
#include "Matrix.h"

GameScene::~GameScene() {
	/*
	for (auto& model : models_) {
		delete model.second;
	}

	for (Object3d* object : objects_) {
		delete object;
	}
	*/
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

	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.3f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 3.0f, -10.0f });

	camera2_ = std::make_unique<Camera>();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(camera2_.get(), input_);

	mainCamera_ = camera_.get();

	textureHandle_[0] = TextureManager::GetInstance()->Load("./resources/uvChecker.dds");
	textureHandle_[1] = TextureManager::GetInstance()->Load("./resources/monsterBall.png");

	soundData1_ = audio_->SoundLoadWave("./resources/Alarm01.wav");

	
	model_ = std::make_unique<Model>();
	model_->Initialize(modelPlatform_);
	model_->CreateModel("./resources/simpleSkin", "simpleSkin.gltf");
	
	animation_ = std::make_unique<Animation>();
	animation_->LoadAnimationFile("./resources/simpleSkin", "simpleSkin.gltf");

	/*
	Model* newModel = new Model();
	newModel->Initialize(modelPlatform_);
	newModel->CreateModel("./resources", "plane.obj");
	models_.insert(std::make_pair("plane", newModel));

	newModel = new Model();
	newModel->Initialize(modelPlatform_);
	newModel->CreateModel("./resources", "axis.obj");
	models_.insert(std::make_pair("axis", newModel));

	LevelData* levelData = LevelDataLoad("./resources/", "levelData", ".json");

	//レベルデータからオブジェクトを生成、配置
	for (ObjectData& objectData : levelData->objects) {
		//ファイルから登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if (it != models_.end())
		{
			model = it->second;
		}
		//モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = new Object3d();
		newObject->Initialize(model, mainCamera_);

		newObject->SetTransform(objectData.transform);
		//配列に登録
		objects_.push_back(newObject);
	}
	*/
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_[0], spritePlatform_);
	sprite_->SetPosition({ 100.0f, 100.0f });

	/*
	for (uint32_t i = 0; i < 5; ++i) {
		std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
		if (i < 2) {
			sprite->Initialize(textureHandle_[i], Vector2{ 100.0f, 100.0f }, spritePlatform_);
		}
		else {
			sprite->Initialize(textureHandle_[i % 2], Vector2{ 100.0f, 100.0f }, spritePlatform_);
		}
		Vector2 position = { 50.0f + 200.0f * i, 100.0f };
		sprite->SetPosition(position);
		sprites_.push_back(std::move(sprite));
	}
	*/

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(model_.get());
	player_->SetAnimation(animation_.get());

	/*
	//3dオブジェクトの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(modelAxis_.get(), mainCamera_);
	*/
	

	//emitter_ = std::make_unique<ParticleEmitter>("circle", 3, 0.5f);
	//emitter_->Initialize(textureHandle_[0]);

}

void GameScene::Update() {

	camera_->Update();

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}

	//アニメーションの時間を進める
	animation_->Update();

	//プレイヤーの更新
	player_->Update();

	//3dオブジェクトの更新
	//object3d_->Update("teapot");

	

	//emitter_->Update();

	//ParticleManager::GetInstance()->Update(mainCamera_);

	ImGui::Begin("Window");
	
	if (ImGui::TreeNode("Sprite")) {
		ImGui::DragFloat2("tranlate", &sprite_->GetPosition().x, 0.01f);
		ImGui::DragFloat2("size", &sprite_->GetSize().x, 0.01f);
		ImGui::SliderAngle("rotate", &sprite_->GetRotation());
		ImGui::ColorEdit4("color", &sprite_->GetColor().x);
		ImGui::DragFloat2("anchprPoint", &sprite_->GetAnchorPoint().x, 0.01f);
		ImGui::Checkbox("IsFlipX", &sprite_->GetIsFlipX());
		ImGui::Checkbox("IsFlipY", &sprite_->GetIsFlipY());
		ImGui::DragFloat2("textureLeftTop", &sprite_->GetTextureLeftTop().x, 0.01f);
		ImGui::DragFloat2("textureSize", &sprite_->GetTextureSize().x, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("SpriteUV")) {
		ImGui::DragFloat2("UVTranslate", &sprite_->GetUVTransformAddress().translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UVScale", &sprite_->GetUVTransformAddress().scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("UVRotate", &sprite_->GetUVTransformAddress().rotate.z);

		ImGui::TreePop();
	}
	
	if (ImGui::TreeNode("camera")) {
		ImGui::DragFloat3("translate", &camera_->GetTranslate().x, 0.01f);
		ImGui::DragFloat3("rotate", &camera_->GetRotate().x, 0.01f);
		//ImGui::DragFloat3("scale", &cameratransform.scale.x, 0.01f);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("camera2")) {
		ImGui::DragFloat3("translate", &camera2_->GetTranslate().x, 0.01f);
		ImGui::DragFloat3("rotate", &camera2_->GetRotate().x, 0.01f);
		//ImGui::DragFloat3("scale", &cameratransform.scale.x, 0.01f);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("directionalLight")) {
		ImGui::ColorEdit4("color", &directionalLight_->GetColor().x);
		ImGui::DragFloat3("direction", &directionalLight_->GetDirection().x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_->GetIntensity(), 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::Button("SE01")) {
		audio_->SoundPlawWave(soundData1_);
	}

	if (ImGui::RadioButton("gameCamera", !isActiveDebugCamera_)) {
		isActiveDebugCamera_ = false;

		mainCamera_ = camera_.get();

		//player_->SetCamera(mainCamera_);
		//object3d_->SetCamera(mainCamera_);
		

	}
	if (ImGui::RadioButton("DebugCamera", isActiveDebugCamera_)) {
		isActiveDebugCamera_ = true;

		mainCamera_ = camera2_.get();

		//player_->SetCamera(mainCamera_);
		//object3d_->SetCamera(mainCamera_);
		

	}
	/*
	ImGui::RadioButton("BlendModeNone", &blendMode, static_cast<int>(BlendMode::kBlendModeNone));
	ImGui::RadioButton("BlendModeNormal", &blendMode, static_cast<int>(BlendMode::kBlendModeNormal));
	ImGui::RadioButton("BlendModeAdd", &blendMode, static_cast<int>(BlendMode::kBlendModeAdd));
	ImGui::RadioButton("BlendModeSubtract", &blendMode, static_cast<int>(BlendMode::kBlendModeSubtract));
	ImGui::RadioButton("BlendModeMultiply", &blendMode, static_cast<int>(BlendMode::kBlendModeMultiply));
	ImGui::RadioButton("BlendModeScreen", &blendMode, static_cast<int>(BlendMode::kBlendModeScreen));
	*/

	//ImGui::Checkbox("useMonsterBall", &useMonaterBall);
	ImGui::End();

	if (input_->PushKey(DIK_0)) {
		OutputDebugStringA("Hit 0\n");
	}
	if (input_->TriggerKey(DIK_1)) {
		OutputDebugStringA("Hit 1\n");
	}

}

void GameScene::Draw() {

	//primitiveDrawer->SetPipelineSet(dxCommon_->GetCommandList(), static_cast<BlendMode>(blendMode));


	//dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, useMonaterBall ? textureSrvHandleGPU2 : textureSrvHandleGPU);

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//プレイヤーの描画
	player_->Draw(mainCamera_);
	//3dオブジェクトの描画
	//object3d_->Draw();
	/*
	for (Object3d* object : objects_) {
		object->Draw();
	}
	*/
	//ParticleManager::GetInstance()->Draw();

	//modelの描画
	//model_->Draw(commandList);

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	//Spriteの描画。変更が必要なものだけ変更する

	/*
	for (const std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
	*/
	//sprite_->Draw();


	modelPlatform_->LinePreDraw();

	player_->BoneDraw(mainCamera_);
	/*
	Matrix4x4 WVP1 = MakeIdentity4x4();
	Matrix4x4 WVP2 = MakeTranslateMatrix({ 2.0f, 0.0f, 0.0f });
	Matrix4x4 WVP3 = MakeTranslateMatrix({ 0.0f, 2.0f, 0.0f });

	modelPlatform_->LineDraw(WVP1, WVP2, mainCamera_);
	modelPlatform_->LineDraw(WVP3, WVP2, mainCamera_);
	*/

	modelPlatform_->SpherePreDraw();

	player_->JointDraw(mainCamera_);
	/*
	modelPlatform_->SphereDraw(WVP1, mainCamera_);
	modelPlatform_->SphereDraw(WVP2, mainCamera_);
	*/
}

void GameScene::Finalize()
{

}
