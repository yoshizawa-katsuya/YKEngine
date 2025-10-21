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
	TextureManager* textureManager = TextureManager::GetInstance();

	//パーティクル
	emitter_ = std::make_unique<ParticleEmitter>("HitEffect01", 3, 1.5f);
	uint32_t textureHandle3 = textureManager->Load("./Resources/circle2.png");
	emitter_->Initialize(textureHandle3, modelPlatform_->CreatePlane(textureHandle3));

	playerSpawnEmitter_ = std::make_unique<ParticleEmitter>("PlayerStartEffect01", 1, 1.0f / 60.0f);
	uint32_t whiteTextureHandle = textureManager->Load("./Resources/white.png");
	playerSpawnEmitter_->Initialize(whiteTextureHandle, modelPlatform_->CreateSphere(whiteTextureHandle));
}

void EffectManager::Update()
{
#ifdef _DEBUG

	emitter_->ApplyGlobalVariables();
	playerSpawnEmitter_->ApplyGlobalVariables();

#endif // _DEBUG
}

void EffectManager::SpawnHitEffect(const Vector3& position)
{
	emitter_->SetTranslation(position); // パーティクルの位置を設定
	emitter_->Emit(); // パーティクルを発生させる
}

void EffectManager::SpawnPlayerStartEffect(const Vector3& position)
{
	playerSpawnEmitter_->SetTranslation(position);
	playerSpawnEmitter_->Emit();
}
