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

std::shared_ptr<BaseModel> EffectManager::LoadEffectModel(std::string modelName, uint32_t textureHnadle)
{
	std::string tag = "Effect";
	if (modelName == "primitivePlane")
	{
		return modelPlatform_->CreatePlane(textureHnadle, tag);
	}
	else if (modelName == "primitiveSphere")
	{
		return modelPlatform_->CreateSphere(textureHnadle, tag);
	}

	assert(false);
	return modelPlatform_->CreatePlane(textureHnadle, tag);
}
