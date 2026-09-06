#include "GameScene.h"
#include "dx12.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "NormalEnemy.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GameScene::~GameScene() 
{
	//衝突マネージャーに登録されたコライダーを削除
	collisionManager_->Reset();
}

void GameScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });

	//デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	//メインカメラの設定
	mainCamera_ = camera_.get();

	//モデルを描画する際カメラの設定は必須
	modelPlatform_->SetCamera(mainCamera_);
	
	//プレイヤー管理クラスの生成
	playerManager_ = std::make_unique<PlayerManager>();
	playerManager_->Initialize();
	
	//衝突マネージャーの取得
	collisionManager_ = CollisionManager::GetInstance();
	//modelPlatform_->SetSpotLight(spotLight_.get());

	enemy_ = std::make_unique<NormalEnemy>();
	enemy_->Initialize();
	enemy_->SetTargets(playerManager_->GetLeftPlayer(), playerManager_->GetRightPlayer());
}

void GameScene::Update() {

	//カメラの更新
	camera_->Update();

	if (isActiveDebugCamera_)
	{
		debugCamera_->Update();
	}

	//プレイヤーの更新
	playerManager_->Update();

	enemy_->Update();

	modelPlatform_->LightPreUpdate();
	modelPlatform_->DirectionalLightUpdate(directionalLight_);
	
	//衝突判定
	collisionManager_->CheckAllCollisions();

	if (input_->TriggerKey(DIK_SPACE)) 
	{
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
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

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("DirectionalLight")) {
		ImGui::ColorEdit4("color", &directionalLight_.color.x);
		ImGui::DragFloat3("direction", &directionalLight_.direction.x, 0.01f);
		ImGui::DragFloat("intensity", &directionalLight_.intensity, 0.01f);

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

	
	ImGui::End();
		

#endif // USE_IMGUI
	

}

void GameScene::Draw() {

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//環境マップを使う場合はコメントアウトを外す
	//TextureManager::GetInstance()->SetEnvironmentMap(textureHandle2_);

	//プレイヤーの描画
	playerManager_->Draw(mainCamera_);

	enemy_->Draw(mainCamera_);

	
	modelPlatform_->InstancingPreDraw();

	//コライダーの描画
	collisionManager_->Draw(mainCamera_);
}

void GameScene::Finalize()
{

}