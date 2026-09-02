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
}

void PlayerManager::Update()
{
	leftPlayer_->Update();
	rightPlayer_->Update();
}

void PlayerManager::Draw(YKEngine::Camera* camera)
{
	leftPlayer_->Draw(camera);
	rightPlayer_->Draw(camera);
}
