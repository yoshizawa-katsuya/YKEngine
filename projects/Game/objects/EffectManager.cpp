#include "EffectManager.h"
#include "ModelPlatform.h"
#include "ParticleManager.h"

using namespace YKEngine;

EffectManager* EffectManager::GetInstance()
{
	static EffectManager instance;
	return &instance;
}

void EffectManager::Initialize()
{
	modelPlatform_ = ModelPlatform::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();

	uint32_t whiteTextureHandle = textureManager->Load("./Resources/white.png");

	//パーティクルエミッター生成
	for (const std::pair<EffectType, EffectData>& effectData : effectDatas_)
	{
		std::unique_ptr<ParticleEmitter>& emitter = effectEmitters_[effectData.first];
		emitter = std::make_unique<ParticleEmitter>(effectData.second.name);
		uint32_t textureHandle = textureManager->Load(effectData.second.textureFilePath);
		std::shared_ptr<BaseModel> model = LoadEffectModel(effectData.second.modelName, whiteTextureHandle);
		emitter->Initialize(textureHandle, model);
	}

}

void EffectManager::Update()
{
#ifdef _DEBUG

	for (const std::pair<const EffectType, std::unique_ptr<ParticleEmitter>>& emitter : effectEmitters_)
	{
		emitter.second->ApplyGlobalVariables();
	}

#endif // _DEBUG
}

void EffectManager::SpawnEffect(EffectType effectType, const Vector3& position)
{
	std::unique_ptr<ParticleEmitter>& emitter = effectEmitters_[effectType];
	emitter->SetTranslation(position); // パーティクルの位置を設定
	emitter->Emit(); // パーティクルを発生させる
}

void EffectManager::SpawnEffect(EffectType effectType, const Vector3& position, uint32_t count)
{
	std::unique_ptr<ParticleEmitter>& emitter = effectEmitters_[effectType];
	emitter->SetTranslation(position); // パーティクルの位置を設定
	for (uint32_t i = 0; i < count; i++)
	{
		emitter->Emit(); // パーティクルを発生させる
	}
}

void EffectManager::SpawnEffect(EffectType effectType, const Vector3& position, const Vector3& velocity, uint32_t count)
{
	std::unique_ptr<ParticleEmitter>& emitter = effectEmitters_[effectType];
	emitter->SetTranslation(position); // パーティクルの位置を設定
	Vector3 particleVelocity = velocity;
	particleVelocity *= 60.0f; // 1秒あたりのフレーム数である60を掛けて、フレームごとの速度に変換
	emitter->SetRandVelocityMin(particleVelocity); // パーティクルの初速の最小値を設定
	emitter->SetRandVelocityMax(particleVelocity); // パーティクルの初速の最大値を設定
	for (uint32_t i = 0; i < count; i++)
	{
		emitter->Emit(); // パーティクルを発生させる
	}
}

void EffectManager::ClearEffects(EffectType effectType)
{
	ParticleManager::GetInstance()->ClearParticles(effectDatas_[effectType].name);
}

void EffectManager::ClearAllEffects()
{
	for (const std::pair<EffectType, EffectData>& effectData : effectDatas_)
	{
		ParticleManager::GetInstance()->ClearParticles(effectData.second.name);
	}
}

std::shared_ptr<BaseModel> EffectManager::LoadEffectModel(const std::string& modelName, uint32_t textureHnadle)
{
	//モデル生成関数の対応表からモデル生成関数を取得して呼び出す
	const std::unordered_map<std::string, EffectModelFactory> effectModelFactoryMap = GetEffectModelFactoryMap();

	auto it = effectModelFactoryMap.find(modelName);
	std::string tag = "Effect";
	if (it != effectModelFactoryMap.end())
	{
		
		return it->second(textureHnadle, tag);
	}

	assert(false);
	return modelPlatform_->CreatePlane(textureHnadle, tag);
}

const std::unordered_map<std::string, EffectManager::EffectModelFactory>& EffectManager::GetEffectModelFactoryMap() const
{
	static const std::unordered_map<std::string, EffectModelFactory> effectModelFactoryMap
	{
		{"primitivePlane", [this](uint32_t textureHandle, const std::string& tag) { return modelPlatform_->CreatePlane(textureHandle, tag); }},
		{"primitiveSphere", [this](uint32_t textureHandle, const std::string& tag) { return modelPlatform_->CreateSphere(textureHandle, tag); }},
		{"primitiveCube", [this](uint32_t textureHandle, const std::string& tag) { return modelPlatform_->CreateCube(textureHandle, tag); }},
	};
	return effectModelFactoryMap;
}
