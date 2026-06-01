#define NOMINMAX
#include "GameScene.h"
#include "dx12.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "LevelDataLoader.h"
#include "JudgeSystem.h"
#include <algorithm>

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GameScene::~GameScene() {
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.16f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 3.6f, -10.0f });

	normalFov_ = camera_->GetFovY();

	//デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	//メインカメラの設定
	mainCamera_ = camera_.get();

	// カメラマネージャーの設定
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize(camera_.get());

	//モデルを描画する際カメラの設定は必須
	modelPlatform_->SetCamera(mainCamera_);

	textureHandle_ = TextureManager::GetInstance()->Load("./resources/white.png");
	textureHandle2_ = TextureManager::GetInstance()->Load("./resources/rostock_laage_airport_4k.dds");

	//モデルの生成
	modelPlayer_ = modelPlatform_->CreateSkinModel("./resources/playerAnimation", "PoseA.gltf");
	modelWall_ = modelPlatform_->CreateRigidModel("./resources/walls", "SquatWall.obj");
	//modelPlayer_->SetUVTransform({ 10.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	//modelPlayer_->SetEnableLighting(false);
	//modelPlayer_ = std::make_unique<RigidModel>();

	/*
	//スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	*/
	backgroundSprite_ = std::make_unique<Sprite>();
	backgroundSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/back.png"));

	//パーティクルエミッターの生成
	//emitter_ = std::make_unique<ParticleEmitter>("Effect", 1, 1.5f);
	//emitter_->Initialize(textureHandle2, modelPlayer_, true);

	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());


	//ダミーの壁の初期化
	dummyWall_=std::make_unique<DummyWall>();
	dummyWall_->Initialize(modelWall_.get());

	//UIの初期化
	ui_ = std::make_unique<Ui>();
	ui_->Initialize();


	effect_ = std::make_unique<Effect>();
	effect_->Initialize();

	/*skyBox_ = std::make_unique<Rigid3dObject>();
	skyBox_->Initialize(modelPlatform_->CreateSkyBox(textureHandle2_).get());
	skyBoxWorldTransform_.Initialize();
	skyBoxWorldTransform_.scale_ = { 50.0f, 50.0f, 50.0f };
	skyBoxWorldTransform_.UpdateMatrix();
	skyBox_->WorldTransformUpdate(skyBoxWorldTransform_);*/

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
	// 遷移演出
	transition_ = std::make_unique<Transition>();

	// 遷移演出の初期化
	transition_->Initialize();

	// ゲーム画面に切り替わったと同時にフェードアウトの画面遷移を開始
	transition_->StartFadeOut(
		TextureManager::GetInstance()->Load("./resources/brickLoad.png"),
		TextureManager::GetInstance()->Load("./resources/brickMask.png"),
		1.0f,
		Transition::EasingType::EaseInSine
	);

	// 難易度の設定
	difficulty_ = sceneManager_->GetDifficulty();
	CreateLevel();

}

void GameScene::Update() {


	//カメラの更新
	camera_->Update();
	cameraManager_->Update();

	switch (cameraMode_) {
	case CameraMode::Free:
		// 通常時はカメラマネージャーの更新のみ
		break;
	case CameraMode::GameOver:
		// ゲームオーバー演出中はカメラをプレイヤーに固定
		cameraManager_->LookAtTarget(player_->GetWorldTransform().translation_);
		break;
	}

	if (isActiveDebugCamera_) {
		debugCamera_->Update();
	}
	// 画面遷移の更新
	transition_->Update();
	// 画面遷移が終わり、次のシーン名が設定されている場合はシーンを切り替える
	if (transition_->IsFinished() &&
		!nextSceneName_.empty()) {

		sceneManager_->ChengeScene(nextSceneName_);
	}

	// プレイヤーがリセットされた瞬間だけ
	if (player_->ConsumeResetRequest())
	{
		cameraMode_ = CameraMode::Free;

		camera_->SetRotate({ 0.16f, 0.0f, 0.0f });
		camera_->SetTranslate({ 0.0f, 3.6f, -10.0f });

		camera_->SetFovY(normalFov_);

		cameraShakeTimer_ = 0.0f;

		isExplosionShakeStarted_ = false;
		isExplosionShakeFinished_ = false;
		explosionShakeTimer_ = 0.0f;
	}

	// ダメージを受けた瞬間だけ
	if (damageShakeTimer_ > 0.0f)
	{
		damageShakeTimer_ -= 1.0f / 60.0f;

		Vector3 pos = damageShakeBasePos_;

		pos.x += (rand() % 100 / 100.0f - 0.5f) * 0.32f;
		pos.y += (rand() % 100 / 100.0f - 0.5f) * 0.05f;

		camera_->SetTranslate(pos);

		// 終了時に元へ戻す
		if (damageShakeTimer_ <= 0.0f)
		{
			camera_->SetTranslate(damageShakeBasePos_);
		}
	}

	// ゲームオーバー演出
	if (player_->IsInHitImpact() || player_->IsDead() || player_->IsDeathFinished()) {
		GameOverAnimation();
	}

	//プレイヤーの更新
	player_->Update();

	// 死亡演出終了検知
	if (player_->IsDeathFinished())
	{
		if (!isDeathFinishedTimerStarted_)
		{
			isDeathFinishedTimerStarted_ = true;
			deathFinishedTimer_ = 1.5f;
		}
	}

	// エフェクト終了待ち
	if (isDeathFinishedTimerStarted_)
	{
		deathFinishedTimer_ -= 1.0f / 60.0f;

		if (deathFinishedTimer_ <= 0.0f &&
			!isStartedTransition_)
		{
			isStartedTransition_ = true;

			nextSceneName_ = "GameOverScene";

			transition_->StartFadeIn(
				TextureManager::GetInstance()->Load("./resources/brickLoad.png"),
				TextureManager::GetInstance()->Load("./resources/brickMask2.png"),
				2.0f,
				Transition::EasingType::EaseOutQuint
			);
		}
	}

	//ダミーの壁の更新
	prevWallZ_ = dummyWall_->GetWorldTransform().translation_.z;
	//dummyWall_->Update();

	//レーンの更新
	laneManager_->Update();

	//衝突判定
	CheckWallCollision();

	//UIの更新
	ui_->Update();

	effect_->Update();

	switch (ui_->GetPauseMenu()) {
	case Ui::PauseMenu::Retry:
		// リトライが選択された場合、ゲームシーンに遷移する
		nextSceneName_ = "GameScene";
		transition_->StartFadeIn(
			TextureManager::GetInstance()->Load("./resources/brickLoad.png"),
			TextureManager::GetInstance()->Load("./resources/brickMask2.png"),
			2.0f,
			Transition::EasingType::EaseOutQuint
		);
		break;

	case Ui::PauseMenu::ToTitle:
		// タイトルに戻るが選択された場合、タイトルシーンに遷移する
		nextSceneName_ = "TitleScene";
		transition_->StartFadeIn(
			TextureManager::GetInstance()->Load("./resources/brickLoad.png"),
			TextureManager::GetInstance()->Load("./resources/brickMask2.png"),
			2.0f,
			Transition::EasingType::EaseOutQuint
		);
		break;
	}

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);
	/*modelPlatform_->PointLightUpdate(pointLight_);
	modelPlatform_->SpotLightUpdate(spotLight_);*/

	/*
	objects_->PreUpdate();
	worldTransform1_.translation_.x += 0.01f;
	worldTransform1_.UpdateMatrix();
	objects_->WorldTransformUpdate(worldTransform1_);
	objects_->WorldTransformUpdate(worldTransform2_);
	*/

	//emitter_->Update();

	ParticleManager::GetInstance()->Update(mainCamera_);

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		nextSceneName_ = "TitleScene";
		transition_->StartFadeIn(
			TextureManager::GetInstance()->Load("./resources/brickLoad.png"),
			TextureManager::GetInstance()->Load("./resources/brickMask2.png"),
			2.0f,
			Transition::EasingType::EaseOutQuint
		);
	}

#ifdef USE_IMGUI


	ImGui::Begin("Window");
	if (ImGui::TreeNode("camera")) {
		Vector3 translate = camera_->GetTranslate();
		ImGui::DragFloat3("translate", &translate.x, 0.01f);
		camera_->SetTranslate(translate);

		Vector3 rotate = camera_->GetRotate();
		ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
		camera_->SetRotate(rotate);
		//ImGui::DragFloat3("scale", &cameratransform.scale.x, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("DirectionalLight")) {
		ImGui::ColorEdit4("color", &directionalLight_.color.x);
		ImGui::DragFloat3("direction", &directionalLight_.direction.x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_.intensity, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("PointLight")) {
		ImGui::ColorEdit4("color", &pointLight_.color.x);
		ImGui::DragFloat3("position", &pointLight_.position.x, 0.01f);
		ImGui::DragFloat("intensity", &pointLight_.intensity, 0.01f);
		ImGui::DragFloat("radius", &pointLight_.radius, 0.01f);
		ImGui::DragFloat("decay", &pointLight_.decay, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("SpotLight")) {
		ImGui::ColorEdit4("color", &spotLight_.color.x);
		ImGui::DragFloat3("position", &spotLight_.position.x, 0.01f);
		ImGui::DragFloat("intensity", &spotLight_.intensity, 0.01f);
		ImGui::DragFloat3("direction", &spotLight_.direction.x, 0.01f);
		ImGui::DragFloat("distance", &spotLight_.distance, 0.01f);
		ImGui::DragFloat("decay", &spotLight_.decay, 0.01f);
		ImGui::DragFloat("cosAngle", &spotLight_.cosAngle, 0.01f);
		ImGui::DragFloat("cosFalloffStart", &spotLight_.cosFalloffStart, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Debug")) {
		ImGui::Text("Score : %d", debugScore_);
		ImGui::Text("Miss  : %d", debugMiss_);
		ImGui::Text("Combo  : %d", debugCombo_);
		ImGui::Text("MaxCombo  : %d", debugMaxCombo_);

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

		mainCamera_ = debugCamera_->GetCamera();
		modelPlatform_->SetCamera(mainCamera_);

	}

	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);
	ImGui::Text("Difficulty: %s", difficulty_ == Difficulty::EASY ? "EASY" : difficulty_ == Difficulty::NORMAL ? "NORMAL" : "HARD");
	// カメラのFOVの調整
	float fov = camera_->GetFovY();
	ImGui::SliderFloat("Camera FOV: %f", &fov, 0.0f, 1.0f);
	camera_->SetFovY(fov);
	ImGui::SliderFloat("GameOver Shake: %f", &shakeStrength_, 0.0f, 1.0f);
	/*
	if (ImGui::Button("BGMstop")) {
		audio_->SoundStopWave(bgm1_);
	}
	*/
	ImGui::End();


#endif // USE_IMGUI
}

void GameScene::Draw() {

	//Spriteの背景描画前処理
	spritePlatform_->PreBackGroundDraw();

	//背景の描画
	backgroundSprite_->Draw();
	//sprite_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	//TextureManager::GetInstance()->SetEnvironmentMap(textureHandle2_);



	//レーンの描画
	laneManager_->Draw(mainCamera_);

	//ダミーの壁の描画
	//dummyWall_->Draw(mainCamera_);

	/*modelPlatform_->SkyBoxPreDraw();

	skyBox_->CameraUpdate(mainCamera_);
	skyBox_->Draw();*/

	/*
	modelPlatform_->InstancingPreDraw();

	objects_->CameraUpdate(mainCamera_);
	objects_->Draw();
	*/

	modelPlatform_->SkinPreDraw();

	//プレイヤーの描画
	player_->Draw(mainCamera_);

	//Spriteの描画前処理
	spritePlatform_->PreDraw();

	//UIの描画
	ui_->Draw();

	// 遷移演出の描画
	transition_->Draw();

	//ParticleManager::GetInstance()->Draw();
	ParticleManager::GetInstance()->Draw();

}

void GameScene::Finalize()
{

}

void GameScene::CheckWallCollision()
{
	//各レーンの壁を取得して判定
	for (uint32_t i = 0; i < static_cast<uint32_t>(PlayerDirection::Count); i++)
	{
		const std::vector<std::unique_ptr<Wall>>& walls = laneManager_->GetWalls(static_cast<PlayerDirection>(i));

		for (const std::unique_ptr<Wall>& wall : walls)
		{
			//壁が衝突済みか、判定ラインに到達していない場合はスキップ
			if (wall->GetIsCollision() || !wall->GetIsLineJudged())
			{
				continue;
			}

			auto result = JudgeSystem::Judge(
				player_->GetState(),
				wall->GetState(),
				player_->GetWorldTransform(),
				wall->GetWorldTransform()
			);

			if (result == JudgeResult::Hit ||
				result == JudgeResult::SuccessSquat) {
				// 成功時の処理
				player_->SetColorForDebug(debugPlayerColor[0]);
				debugScore_++;
				debugCombo_++;
				debugMaxCombo_ = std::max(debugMaxCombo_, debugCombo_);

				if (debugCombo_ >= 10) {
					ui_->PlayJudgeEffect(Ui::JudgeType::Perfect);
					ui_->AddGameScore(1000);
				}
				else if (debugCombo_ >= 5) {
					ui_->PlayJudgeEffect(Ui::JudgeType::Great);
					ui_->AddGameScore(500);
				}
				else {
					ui_->PlayJudgeEffect(Ui::JudgeType::Good);
					ui_->AddGameScore(100);
				}
			}
			else if (result == JudgeResult::Miss) {
				// ミス時の処理
				//player_->SetColorForDebug(debugPlayerColor[1]);
				player_->StartDamageReaction();
				damageShakeTimer_ = 0.15f;
				damageShakeBasePos_ = camera_->GetTranslate();
				debugMiss_++;
				debugCombo_ = 0;

				ui_->DamageLife();
				ui_->StopFrameGlow();

				if (ui_->GetLife() <= 1) {
					player_->RequestDeath();
				}
			}

			wall->SetIsCollision(true); 

		}
	}
}

void GameScene::CreateLevel()
{
	// 難易度に応じたレベルデータの読み込み
	LevelData levelData;
	switch (difficulty_)
	{
	case Difficulty::EASY:
		levelData = LevelDataLoad("./resources/stageData/", "easyStageData", ".json");
		break;
	case Difficulty::NORMAL:
		levelData = LevelDataLoad("./resources/stageData/", "normalStageData", ".json");
		break;
	case Difficulty::HARD:
		levelData = LevelDataLoad("./resources/stageData/", "hardStageData", ".json");
		break;
	}

	//レーンの初期化
	laneManager_ = std::make_unique<LaneManager>();
	laneManager_->Initialize(levelData.walls);

}

void GameScene::GameOverAnimation()
{
	const float deltaTime = 1.0f / 60.0f;

	//----------------------------------------
	// HitImpact中
	//----------------------------------------
	if (player_->IsInHitImpact())
	{
		// ズーム
		float currentFov = camera_->GetFovY();
		float targetFov = 0.5f;

		currentFov += (targetFov - currentFov) * 0.60f;
		camera_->SetFovY(currentFov);

		// 横揺れ
		cameraShakeTimer_ += deltaTime * 80.0f;

		Vector3 pos = camera_->GetTranslate();

		pos.x = std::sin(cameraShakeTimer_) * 0.15f;

		camera_->SetTranslate(pos);
	}

	//----------------------------------------
	// Dead中
	//----------------------------------------
	else if (player_->IsDead())
	{
		// FOV戻し
		float currentFov = camera_->GetFovY();

		currentFov += (normalFov_ - currentFov) * 0.05f;

		camera_->SetFovY(normalFov_);

		cameraMode_ = CameraMode::GameOver;

		//cameraShakeTimer_ = 0.0f;
	}
	else if (player_->IsDeathFinished()) {
		// カメラが揺れる
		// 初回だけ
		if (!isExplosionShakeStarted_ && !isExplosionShakeFinished_)
		{
			isExplosionShakeStarted_ = true;
			explosionShakeTimer_ = explosionShakeDuration_;
		}

		// タイマー減少
		explosionShakeTimer_ -= deltaTime;

		float t = explosionShakeTimer_ / explosionShakeDuration_;
		t = std::clamp(t, 0.0f, 1.0f);

		float strength = 3.0f * t * t;

		cameraShakeTimer_ += deltaTime * 45.0f;

		Vector3 basePos = { 0.0f, 3.6f, -10.0f };
		Vector3 pos = basePos;

		pos.x += std::sin(cameraShakeTimer_) * strength;
		pos.y += std::cos(cameraShakeTimer_ * 1.7f) * strength * 0.35f;

		camera_->SetTranslate(pos);

		// 終了
		if (explosionShakeTimer_ <= 0.0f)
		{
			camera_->SetTranslate(basePos);

			isExplosionShakeStarted_ = false;
			isExplosionShakeFinished_ = true;
		}
	}
}