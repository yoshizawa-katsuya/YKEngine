#include "DemoPlayer.h"
#include "manager/EffectManager.h"

void DemoPlayer::Initialize(BaseModel* model, WorldTransform* parent)
{
	// NULLポインタチェック
	assert(model);

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.translation_ = { 0.0f, 0.0f, -50.0f };

	animator_ = std::make_unique<SRTAnimator>();
	animator_->SetAnimation(worldTransform_.translation_, { 0.0f, 0.0f, 0.0f }, 2);

	Update();
	
}

void DemoPlayer::Update()
{
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);

	switch (phase_)
	{
	case DemoPlayer::Phase::Start:
		UpdateStart();
		break;
	case DemoPlayer::Phase::Main:
		UpdateMain();
		break;
	case DemoPlayer::Phase::End:
		UpdateEnd();
		break;
	default:
		break;
	}

}

void DemoPlayer::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}

void DemoPlayer::SceneEnd()
{
	phase_ = Phase::End;
	animator_->SetAnimation(worldTransform_.translation_, { 0.0f, 0.0f, 50.0f }, 2);
}

void DemoPlayer::UpdateStart()
{
	worldTransform_.translation_ = animator_->Update();
	if (animator_->GetIsEnd())
	{
		phase_ = Phase::Main;

		EffectManager::GetInstance()->SpawnEffect(EffectType::PlayerEndEffect01, worldTransform_.GetWorldPosition(), 30);
	}
}

void DemoPlayer::UpdateMain()
{
}

void DemoPlayer::UpdateEnd()
{
	worldTransform_.translation_ = animator_->Update();
}
