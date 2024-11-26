#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "RigidModel.h"
#include <Vector.h>

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
	camera_->SetTranslate({ 0.0f, 1.75f, -10.0f });

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

	textureHandle_ = TextureManager::GetInstance()->Load("./resources/white.png");
	bossHPBar_textureHandle_ = TextureManager::GetInstance()->Load("./resources/BossHPBar.png");
	bossHP_textureHandle_ = TextureManager::GetInstance()->Load("./resources/BossHP.png");

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

	modelSkydome_ = std::make_unique<RigidModel>();
	modelSkydome_->CreateModel("./resources/skydome", "skydome.obj");

	modelGround_ = std::make_unique<RigidModel>();
	modelGround_->CreateModel("./resources/ground", "ground.obj");



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

	// ボスのHPゲージのスプライト生成
	bossHPBarSprite_ = std::make_unique<Sprite>();
	bossHPBarSprite_->Initialize(bossHPBar_textureHandle_, spritePlatform_);

	bossHPSprite_ = std::make_unique<Sprite>();
	bossHPSprite_->Initialize(bossHP_textureHandle_, spritePlatform_);

	// ボスのロックオン処理の生成
	playerLockOn_ = std::make_unique<PlayerLockOn>();
	playerLockOn_->Initialize(camera_.get());
	boss_->SetLockOn(playerLockOn_.get());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialzie(modelSkydome_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialzie(modelGround_.get());


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
	playerLockOn_->Update(player_);

	// 天球の更新
	skydome_->Update(mainCamera_);

	// 地面の更新
	ground_->Update(mainCamera_);

	bossHP_size_ = bossHPSprite_->GetTextureSize();
	// HPに応じてsize_.xを更新
	bossHP_size_.x = 1280.0f * static_cast<float>(boss_->GetBossHP()) / boss_->GetBossMaxHP();
	// 当たり判定
	CheckAllCollisions();

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
	bossHPBar_position_ = bossHPBarSprite_->GetPosition();
	ImGui::DragFloat2("bossHPBar_position", &bossHPBar_position_.x, 0.1f);
	bossHPBarSprite_->SetPosition(bossHPBar_position_);

	bossHP_position_ = bossHPSprite_->GetPosition();
	ImGui::DragFloat2("bossHP_position", &bossHP_position_.x, 0.1f);
	ImGui::DragFloat("bossHP_size.x", &bossHP_size_.x, 1.0f,0.0f,1000.0f);
	bossHPSprite_->SetPosition(bossHP_position_);
	bossHPSprite_->SetTextureSize(bossHP_size_);
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

	

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//modelPlatform_->SkinPreDraw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	boss_->Draw(mainCamera_);

	// 天球の描画
	skydome_->Draw();

	// 地面の描画
	ground_->Draw();

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	//sprite_->Draw();
	bossHPBarSprite_->Draw();

	bossHPSprite_->Draw();
}

void GameScene::Finalize()
{

}

void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA{}, posB{};

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = GetEnemyBullets();

#pragma region 自キャラと敵弾の当たり判定
	
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵キャラと自弾全ての当たり判定

	for (PlayerBullet* bullet : playerBullets) {
		// 敵キャラの座標
		posA = boss_->GetWorldPosition();
		// 自弾の座標
		posB = bullet->GetWorldPosition();
		// 距離
		float length = Length(Subtract(posB, posA));
		if (length <= (playerBulletRadius_ + bossRadius_)) {
			// 敵キャラの衝突時コールバックを呼び出す
			boss_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	
#pragma endregion
}

