#include "DemoPlayer.h"
#include "manager/EffectManager.h"
#include "ModelPlatform.h"
#include "DemoPlayerStartState.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void DemoPlayer::Initialize(WorldTransform* parent)
{
	globalVariables_ = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::DemoPlayer::kGroupName;
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, JsonKey::DemoPlayer::kStartAnimeDuration, 2.0f);
	globalVariables_->AddItem(groupName, JsonKey::DemoPlayer::kInitialPosition, Vector3{ 0.0f, 0.0f, -50.0f });
	globalVariables_->AddItem(groupName, JsonKey::DemoPlayer::kEndAnimeDuration, 2.0f);
	globalVariables_->AddItem(groupName, JsonKey::DemoPlayer::kEndAnimeTranslate, Vector3{ 0.0f, 0.0f, 50.0f });

	object_ = std::make_unique<My3dObject>();
	object_->Initialize(ModelPlatform::GetInstance()->CreateRigidModel("./Resources/player", "Player.obj").get());

	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.translation_ = globalVariables_->GetVector3Value(groupName, JsonKey::DemoPlayer::kInitialPosition);

	animator_ = std::make_unique<SRTAnimator>();
	animator_->SetAnimation(worldTransform_.translation_, { 0.0f, 0.0f, 0.0f }, globalVariables_->GetFloatValue(groupName, JsonKey::DemoPlayer::kStartAnimeDuration));

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
	const float duration = globalVariables_->GetFloatValue(JsonKey::DemoPlayer::kGroupName, JsonKey::DemoPlayer::kEndAnimeDuration);
	const Vector3 targetTranslate = globalVariables_->GetVector3Value(JsonKey::DemoPlayer::kGroupName, JsonKey::DemoPlayer::kEndAnimeTranslate);
	animator_->SetAnimation(worldTransform_.translation_, targetTranslate, duration);
}
