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

	object_ = std::make_unique<Skin3dObject>();
	object_->Initialize(ModelPlatform::GetInstance()->CreateSkinModel("./Resources/player", "Player.gltf").get());

	animation_ = std::make_unique<Animation>();
	animation_->LoadAnimationFile("./Resources/player/", "Player.gltf");

	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.translation_ = globalVariables_->GetVector3Value(groupName, JsonKey::DemoPlayer::kInitialPosition);

	//接近アニメーションのアニメーターを初期化
	accessAnimator_ = std::make_unique<SRTAnimator>();
	const Vector3 accessEnd = { 0.0f, 0.0f, 0.0f };
	accessAnimator_->SetAnimation(worldTransform_.translation_, accessEnd, globalVariables_->GetFloatValue(groupName, JsonKey::DemoPlayer::kStartAnimeDuration));
	//往復アニメーションのアニメーターを初期化
	pingPongAnimator_ = std::make_unique<SRTAnimator>();
	const Vector3 pingPongStart = { 0.0f, -1.0f, 0.0f };
	pingPongAnimator_->SetAnimation(pingPongStart, { 0.0f, 1.0f, 0.0f }, 2.0f, true);
	pingPongAnimator_->SetEasingType(EasingType::kEaseInOutSine);
	//往復アニメーションの開始地点とdemoPlayerの位置を補正するためのアニメーターを初期化
	correctionAnimator_ = std::make_unique<SRTAnimator>();
	correctionAnimator_->SetAnimation(accessEnd, pingPongStart, 1.0f);
	correctionAnimator_->SetEasingType(EasingType::kEaseOutSine);

	//ステートマシンの初期化
	stateMachine_ = std::make_unique<StateMachine<DemoPlayerStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<DemoPlayerStartState>();

	Update();
	
}

void DemoPlayer::Update()
{
	animation_->Update();
	object_->AnimationUpdate(animation_.get());

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
	worldTransform_.translation_ = accessAnimator_->Update();
}

void DemoPlayer::UpdateMain()
{
	if (correctionAnimator_->GetIsEnd())
	{
		worldTransform_.translation_ = pingPongAnimator_->Update();
	}
	else 
	{
		worldTransform_.translation_ = correctionAnimator_->Update();
	}
}

void DemoPlayer::UpdateEnd()
{
	worldTransform_.translation_ = accessAnimator_->Update();
}

void DemoPlayer::AfterStartComplete()
{
	EffectManager::GetInstance()->SpawnEffect(EffectType::kDemoPlayer01, worldTransform_.GetWorldPosition());
}

void DemoPlayer::BeforeEnd()
{
	const float duration = globalVariables_->GetFloatValue(JsonKey::DemoPlayer::kGroupName, JsonKey::DemoPlayer::kEndAnimeDuration);
	const Vector3 targetTranslate = globalVariables_->GetVector3Value(JsonKey::DemoPlayer::kGroupName, JsonKey::DemoPlayer::kEndAnimeTranslate);
	accessAnimator_->SetAnimation(worldTransform_.translation_, targetTranslate, duration);
}
