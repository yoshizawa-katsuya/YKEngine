#include "PlayerManager.h"
#include "ModelPlatform.h"

using namespace YKEngine;

void PlayerManager::Initialize()
{
	//モデルの生成
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	std::shared_ptr<BaseModel> modelPlayer = modelPlatform->CreateRigidModel("./resources/Player", "Player.obj");

	//プレイヤーの初期化
	leftPlayer_ = std::make_unique<LeftPlayer>();
	leftPlayer_->Initialize(modelPlayer.get());

	rightPlayer_ = std::make_unique<RightPlayer>();
	rightPlayer_->Initialize(modelPlayer.get());

	// プレイヤー同士の参照を設定
	leftPlayer_->SetOtherPlayerWorldTransform(rightPlayer_->GetWorldTransform());
	rightPlayer_->SetOtherPlayerWorldTransform(leftPlayer_->GetWorldTransform());

	//レーザーの初期化
	laser_ = std::make_unique<Laser>();
	laser_->Initialize(leftPlayer_->GetWorldTransform(), rightPlayer_->GetWorldTransform());

}

void PlayerManager::Update()
{
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
