#pragma once
#include <memory>
#include "ParticleEmitter.h"
class ModelPlatform;
class BaseModel;

enum class EffectType
{
	HitEffect01,
	PlayerStartEffect01,
	PlayerEndEffect01,
};

/// <summary>
/// エフェクト管理クラス。
/// エフェクトの生成を行う。
/// パーティクルを使用。
/// エフェクトの生成はこのクラスを通して行う。
/// </summary>
class EffectManager
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>エフェクト管理クラスのインスタンス</returns>
	static EffectManager* GetInstance();

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// エフェクトの生成。
	/// </summary>
	/// <param name="position">生成位置</param>
	void SpawnEffect(EffectType effectType, const Vector3& position);

	/// <summary>
	/// エフェクトの生成。
	/// </summary>
	/// <param name="effectType">エフェクトの種類</param>
	/// <param name="position">生成位置</param>
	/// <param name="count">生成数</param>
	void SpawnEffect(EffectType effectType, const Vector3& position, uint32_t count);

private:

	EffectManager() = default;
	~EffectManager() = default;
	EffectManager(EffectManager&) = delete;
	const EffectManager& operator=(EffectManager&) = delete;

	std::shared_ptr<BaseModel> LoadEffectModel(std::string modelName, uint32_t textureHnadle);

	struct EffectData
	{
		std::string name;
		std::string textureFilePath;
		std::string modelName;
	};

	std::unordered_map<EffectType, EffectData> effectDatas_
	{
		{{EffectType::HitEffect01}, {"HitEffect01", "./Resources/circle2.png", "primitivePlane"}},
		{{EffectType::PlayerStartEffect01}, {"PlayerStartEffect01", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::PlayerEndEffect01}, {"PlayerEndEffect01", "./Resources/white.png", "primitiveSphere"}},
	};

	//パーティクル
	std::unordered_map<EffectType, std::unique_ptr<ParticleEmitter>> effectEmitters_;

	ModelPlatform* modelPlatform_ = nullptr;
};

