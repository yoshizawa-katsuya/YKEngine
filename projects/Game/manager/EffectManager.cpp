#include "EffectManager.h"
#include "ModelPlatform.h"

EffectManager* EffectManager::GetInstance()
{
	static EffectManager instance;
	return &instance;
}

void EffectManager::Initialize()
{
	modelPlatform_ = ModelPlatform::GetInstance();

	//パーティクル
	emitter_ = std::make_unique<ParticleEmitter>("HitEffect01", 3, 1.5f);
	uint32_t textureHandle3 = TextureManager::GetInstance()->Load("./Resources/circle2.png");
	emitter_->Initialize(textureHandle3, modelPlatform_->CreatePlane(textureHandle3));
	emitter_->SetScale({ 0.1f, 2.0f, 2.0f });
	emitter_->SetIsRandomScele(true);
	emitter_->SetIsRandomRotate(true);
	emitter_->SetRandRotateMax({ 0.0f, 0.0f, std::numbers::pi_v<float> });
	emitter_->SetRandRotateMin({ 0.0f, 0.0f, -std::numbers::pi_v<float> });
	emitter_->SetRandScaleMax({ 0.0f, 1.0f, 0.0f });
	emitter_->SetRandScaleMin({ 0.0f, -0.6f, 0.0f });
}

void EffectManager::SpawnHitEffect(const Vector3& position)
{
	emitter_->SetTranslation(position); // パーティクルの位置を設定
	emitter_->Emit(); // パーティクルを発生させる
}
