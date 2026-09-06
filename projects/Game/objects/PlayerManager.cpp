#include "PlayerManager.h"
#include "ModelPlatform.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void PlayerManager::Initialize()
{
	//モデルの生成
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	std::shared_ptr<BaseModel> modelLeftPlayer = modelPlatform->CreateRigidModel("./resources/leftPlayer", "leftPlayer.obj");
	std::shared_ptr<BaseModel> modelRightPlayer = modelPlatform->CreateRigidModel("./resources/rightPlayer", "rightPlayer.obj");

	//プレイヤーの初期化
	leftPlayer_ = std::make_unique<LeftPlayer>();
	leftPlayer_->Initialize(modelLeftPlayer.get(), &hp_);

	rightPlayer_ = std::make_unique<RightPlayer>();
	rightPlayer_->Initialize(modelRightPlayer.get(), &hp_);

	// プレイヤー同士の参照を設定
	leftPlayer_->SetOtherPlayerWorldTransform(rightPlayer_->GetWorldTransform());
	rightPlayer_->SetOtherPlayerWorldTransform(leftPlayer_->GetWorldTransform());

	//レーザーの初期化
	laser_ = std::make_unique<Laser>();
	laser_->Initialize(leftPlayer_->GetWorldTransform(), rightPlayer_->GetWorldTransform());

}

void PlayerManager::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("Player");
	ImGui::Text("HP: %d", hp_);
	ImGui::End();

#endif // USE_IMGUI

	//プレイヤーの更新
	leftPlayer_->Update();
	rightPlayer_->Update();

	//レーザーの更新
	laser_->Update();
}

void PlayerManager::Draw(YKEngine::Camera* camera)
{
	//プレイヤーの描画
	leftPlayer_->Draw(camera);
	rightPlayer_->Draw(camera);

	//レーザーの描画
	laser_->Draw(camera);
}
