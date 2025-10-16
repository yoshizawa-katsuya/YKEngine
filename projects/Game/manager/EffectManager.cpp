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
}

void EffectManager::Update()
{
#ifdef _DEBUG

	emitter_->ApplyGlobalVariables();

#endif // _DEBUG
}

void EffectManager::SpawnHitEffect(const Vector3& position)
{
	emitter_->SetTranslation(position); // パーティクルの位置を設定
	emitter_->Emit(); // パーティクルを発生させる
}
