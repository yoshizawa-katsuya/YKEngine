#include "DemoPlayer.h"
#include "manager/EffectManager.h"
#include "ModelPlatform.h"
#include "DemoPlayerStartState.h"

using namespace YKEngine;

void DemoPlayer::Initialize(WorldTransform* parent)
{
	object_ = std::make_unique<My3dObject>();
	object_->Initialize(ModelPlatform::GetInstance()->CreateRigidModel("./Resources/player", "Player.obj").get());

	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.translation_ = { 0.0f, 0.0f, -50.0f };

	animator_ = std::make_unique<SRTAnimator>();
	animator_->SetAnimation(worldTransform_.translation_, { 0.0f, 0.0f, 0.0f }, 2);

	//ステートマシンの初期化
	stateMachine_ = std::make_unique<StateMachine<DemoPlayerStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<DemoPlayerStartState>();

	Update();
	
}

void DemoPlayer::Update()
{
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);

	stateMachine_->Update();

}

void DemoPlayer::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}

void DemoPlayer::UpdateStart()
{
	worldTransform_.translation_ = animator_->Update();
}

void DemoPlayer::UpdateMain()
{
}

void DemoPlayer::UpdateEnd()
{
	worldTransform_.translation_ = animator_->Update();
}

void DemoPlayer::AfterStartComplete()
{
	EffectManager::GetInstance()->SpawnEffect(EffectType::kEnemyEnd01, worldTransform_.GetWorldPosition(), 100);
}

void DemoPlayer::BeforeEnd()
{
	animator_->SetAnimation(worldTransform_.translation_, { 0.0f, 0.0f, 50.0f }, 2);
}
