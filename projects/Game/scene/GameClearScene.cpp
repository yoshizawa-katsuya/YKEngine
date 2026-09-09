#include "GameClearScene.h"
#include "AudioManager.h"
#include "TransformHelpers.h"

#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

#include "input.h"
using namespace YKEngine;

GameClearScene::~GameClearScene() {
	//BGMの停止
	audioManager_->StopBGM(BGMType::kClear);
}

void GameClearScene::Initialize() {
	// デバイスの初期化
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//オーディオマネージャの取得
	audioManager_ = AudioManager::GetInstance();
	//BGMの再生
	audioManager_->PlayBGM(BGMType::kClear);

	// 状態初期化
	select_ = SELECT::kRetry;
	isDecided_ = false;

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });

	//メインカメラの設定
	mainCamera_ = camera_.get();

	//モデルを描画する際カメラの設定は必須
	modelPlatform_->SetCamera(mainCamera_);

	// 背景モデルの生成
	skySphereModel_ = modelPlatform_->CreateRigidModel("./resources/Background", "background.obj");
	skySphereObject_ = std::make_unique<My3dObject>();
	skySphereObject_->Initialize(skySphereModel_.get());
	skySphereTransform_.Initialize();
	skySphereTransform_.scale_ = { 10.0f, 10.0f, 10.0f };
	skySphereObject_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	backgroundModel_ = modelPlatform_->CreateRigidModel("./resources/Background", "wireframe.obj");
	backgroundObject_ = std::make_unique<My3dObject>();
	backgroundObject_->Initialize(backgroundModel_.get());
	backgroundObject_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	backgroundTransform_.Initialize();
	backgroundTransform_.scale_ = { 1.0f, 1.0f, 1.0f };

	gameClearPos_ = { 680.0f, 94.0f };
	// GameClearSprite_の生成
	gameClearSprite_ = std::make_unique<YKEngine::Sprite>();
	gameClearSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/GameClear/GameClear.png"));
	gameClearSprite_->SetPosition(gameClearPos_);
	gameClearSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	returnPos_ = { 540.0f, 541.0f };
	returnSprite_ = std::make_unique<YKEngine::Sprite>();
	returnSprite_->Initialize(TextureManager::GetInstance()->Load("./resources/GameOver/Title.png"));
	returnSprite_->SetPosition(returnPos_);
	returnSprite_->SetColor({ 0.9f, 0.9f, 0.9f, 1.0f });

	leftPlayerModel_ = modelPlatform_->CreateRigidModel("./resources/leftPlayer", "leftPlayer.obj");
	rightPlayerModel_ = modelPlatform_->CreateRigidModel("./resources/rightPlayer", "rightPlayer.obj");

	leftPlayerObject_ = std::make_unique<My3dObject>();
	rightPlayerObject_ = std::make_unique<My3dObject>();

	leftPlayerObject_->Initialize(leftPlayerModel_.get());
	rightPlayerObject_->Initialize(rightPlayerModel_.get());

	leftPlayerTransform_.Initialize();
	rightPlayerTransform_.Initialize();

	leftPlayerTransform_.translation_ = { -30.0f, 2.0f, 0.0f };
	rightPlayerTransform_.translation_ = { -30.0f, -2.0f,0.0f };

	leftPlayerTransform_.scale_ = { 0.5f, 0.5f, 0.5f };
	rightPlayerTransform_.scale_ = { 0.5f, 0.5f, 0.5f };

	//レーザーの初期化
	laser_ = std::make_unique<Laser>();
	laser_->Initialize(&leftPlayerTransform_, &rightPlayerTransform_);

	sceneTransition_.Outro("./resources/white.png");
}

void GameClearScene::Update()
{
	
	// 遷移演出が終わったらシーンを切り替える
	if (sceneTransition_.ConsumeFinished())
	{
		sceneManager_->ChengeScene(
			sceneTransition_.GetTargetScene()
		);

		return;
	}
	camera_->Update();
	
	UpdateDecide();

	sceneTransition_.Update();

	skySphereTransform_.UpdateMatrix();

	skySphereObject_->WorldTransformUpdate(skySphereTransform_);

	backgroundTransform_.rotation_.x += 0.001f;
	backgroundTransform_.rotation_.y += 0.001f;

	backgroundTransform_.UpdateMatrix();

	backgroundObject_->WorldTransformUpdate(backgroundTransform_);


	// プレイヤーアニメーション
	UpdatePlayerAnimation();

	Rotate();

	leftPlayerTransform_.UpdateMatrix();
	rightPlayerTransform_.UpdateMatrix();

	leftPlayerObject_->WorldTransformUpdate(leftPlayerTransform_);
	rightPlayerObject_->WorldTransformUpdate(rightPlayerTransform_);

	//レーザーの更新
	laser_->Update();

	// ライト更新
	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);

	// 経過時間を加算
	pressKeyTimer_ += 1.0f / 60.0f;

	// sinカーブでアルファ値を0.3〜1.0の間で滑らかに往復させる
	float t = sinf(pressKeyTimer_ * kFlickerSpeed);
	float alpha = kAlphaFlickerMin + (kAlphaStable - kAlphaFlickerMin) * (t * 0.5f + 0.5f);

	// 色はそのまま、アルファだけ変化させる
	returnSprite_->SetColor({ kColorPressKey.x, kColorPressKey.y, kColorPressKey.z, alpha });

	DrawImGui();
}

void GameClearScene::Draw()
{
	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	//TextureManager::GetInstance()->SetEnvironmentMap(textureHandle2_);
	// 天球の描画
	skySphereObject_->CameraUpdate(mainCamera_);
	skySphereObject_->Draw();

	backgroundObject_->CameraUpdate(mainCamera_);
	backgroundObject_->Draw();
	leftPlayerObject_->CameraUpdate(mainCamera_);
	rightPlayerObject_->CameraUpdate(mainCamera_);

	leftPlayerObject_->Draw();
	rightPlayerObject_->Draw();

	//レーザーの描画
	laser_->Draw(mainCamera_);
	
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	gameClearSprite_->Draw();
	returnSprite_->SetPosition(returnPos_);
	returnSprite_->Draw();
	sceneTransition_.Draw();
}

void GameClearScene::DrawImGui()
{
	// ImGuiの描画
#ifdef USE_IMGUI
	ImGui::Begin("SELECT");
	ImGui::Text("-> Title");
	ImGui::SliderFloat2("titlePos", &returnPos_.x, 0.0f, 1280.0f);
	ImGui::End();
#endif
}

void GameClearScene::Finalize()
{
}



void GameClearScene::UpdateDecide()
{
	Input* input = Input::GetInstance();

	// 決定ボタン（スペース）
	bool decide = input->TriggerKey(DIK_SPACE);

	if (!decide) {
		return;
	}

	// 以降の入力を無視する（多重入力防止）
	isDecided_ = true;
	// 遷移演出開始
	if (playerAnimationState_ != PlayerAnimationState::kLeaving)
	{
		playerAnimationStartLeft_ = leftPlayerTransform_.translation_;

		playerAnimationStartRight_ = rightPlayerTransform_.translation_;

		playerAnimationState_ = PlayerAnimationState::kLeaving;
		playerAnimationTimer_ = 0.0f;

		audioManager_->PlaySE(SEType::kDecision01);
	}
}

void GameClearScene::UpdatePlayerAnimation()
{
	const float deltaTime = 1.0f / 60.0f;

	playerAnimationTimer_ += deltaTime;

	switch (playerAnimationState_)
	{
	case PlayerAnimationState::kEntering:
	{
		//========================================
		// 左から画面内へ
		//========================================

		float t = playerAnimationTimer_ / 2.0f;
		t = std::clamp(t, 0.0f, 1.0f);

		float easedT = EaseOutCubic(t);

		float x = std::lerp(
			-30.0f,
			0.0f,
			easedT
		);

		leftPlayerTransform_.translation_.x = x;
		rightPlayerTransform_.translation_.x = x;

		// 二機の上下位置
		leftPlayerTransform_.translation_.y = 0.5f;
		rightPlayerTransform_.translation_.y = -0.5f;

		if (t >= 1.0f)
		{
			playerAnimationStartLeft_ = leftPlayerTransform_.translation_;
			playerAnimationStartRight_ = rightPlayerTransform_.translation_;

			playerAnimationState_ = PlayerAnimationState::kIdle;
			playerAnimationTimer_ = 0.0f;
		}

		break;
	}

	case PlayerAnimationState::kIdle:
	{


		// 円周を回りながら徐々に大きくなる
		//========================================

		const float startRadius = 0.5f;
		const float endRadius = 2.5f;

		const float angularSpeed = 1.5f;

		const float centerX = 0.0f;
		const float centerY = 0.0f;

		// 円の拡大にかかる時間
		const float expandDuration = 2.0f;

		// 経過時間
		float expandT = playerAnimationTimer_ / expandDuration;
		expandT = std::clamp(expandT, 0.0f, 1.0f);

		// 半径を0.5 → 2.5へ
		float radius = std::lerp(
			startRadius,
			endRadius,
			expandT
		);

		// 90度から開始
		float angle =
			playerAnimationTimer_ * angularSpeed
			+ 3.14159265f * 0.5f;

		// 左プレイヤー
		leftPlayerTransform_.translation_.x =
			centerX + std::cosf(angle) * radius;

		leftPlayerTransform_.translation_.y =
			centerY + std::sinf(angle) * radius;

		// 右プレイヤーは180度反対
		float rightAngle = angle + 3.14159265f;

		rightPlayerTransform_.translation_.x =
			centerX + std::cosf(rightAngle) * radius;

		rightPlayerTransform_.translation_.y =
			centerY + std::sinf(rightAngle) * radius;

		break;
	}

	case PlayerAnimationState::kLeaving:
	{
		//========================================
		// 右へ飛び去る
		//========================================

		float t = playerAnimationTimer_ / 2.0f;
		t = std::clamp(t, 0.0f, 1.0f);

		// 最初はゆっくり → 最後は高速
		float easedT = EaseInOutSine(t);

		float x = std::lerp(
			playerAnimationStartLeft_.x,
			20.0f,
			easedT
		);


		leftPlayerTransform_.translation_.x = x;

		float rightX = std::lerp(
			playerAnimationStartRight_.x,
			10.0f,
			easedT
		);

		rightPlayerTransform_.translation_.x = rightX;

		float leftY = std::lerp(
			playerAnimationStartLeft_.y,
			4.0f,
			easedT
		);

		float rightY = std::lerp(
			playerAnimationStartRight_.y,
			-4.0f,
			easedT
		);

		leftPlayerTransform_.translation_.y = leftY;
		rightPlayerTransform_.translation_.y = rightY;

		if (t >= 1.0f && !sceneTransition_.IsTransitioning() && !sceneTransition_.IsFinished())
		{
			sceneTransition_.Intro("TitleScene", "./resources/white.png");
		}

		break;
	}
	}
}

void GameClearScene::Rotate()
{
	// 自分ではない方の自機を向く
	Vector3 direction = leftPlayerTransform_.GetWorldPosition() - rightPlayerTransform_.GetWorldPosition();	// 自分ではない方の自機への方向ベクトル
	Vector3 targetRotate = TransformHelpers::FaceToVelocityDirection(rightPlayerTransform_.rotation_, direction);	// 方向ベクトルから回転角を取得

	rightPlayerTransform_.rotation_ = TransformHelpers::NormalizeAngle(targetRotate);

	direction = rightPlayerTransform_.GetWorldPosition() - leftPlayerTransform_.GetWorldPosition();	// 自分ではない方の自機への方向ベクトル
	targetRotate = TransformHelpers::FaceToVelocityDirection(leftPlayerTransform_.rotation_, direction);	// 方向ベクトルから回転角を取得

	leftPlayerTransform_.rotation_ = TransformHelpers::NormalizeAngle(targetRotate);
}

float GameClearScene::EaseOutCubic(float t)
{
	t = std::clamp(t, 0.0f, 1.0f);

	return 1.0f - std::pow(1.0f - t, 3.0f);
}

float GameClearScene::EaseInOutSine(float t)
{
	t = std::clamp(t, 0.0f, 1.0f);

	return -(std::cos(3.14159265f * t) - 1.0f) * 0.5f;
}

float GameClearScene::EaseOutQuint(float t)
{
	t = std::clamp(t, 0.0f, 1.0f);

	return 1.0f - std::pow(1.0f - t, 5.0f);
}