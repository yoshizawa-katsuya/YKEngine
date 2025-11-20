#pragma once
#include <memory>
#include "ParticleEmitter.h"
class ModelPlatform;
class BaseModel;

enum class EffectType
{
	kHit01,
	kGather01,	//集まるエフェクト
	kScatter01,	//散らばるエフェクト
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
	/// 終了処理。
	/// </summary>
	void Finalize();

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
	// シングルトンインスタンス
	static EffectManager* instance_;

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
		{{EffectType::kHit01}, {"HitEffect01", "./Resources/circle2.png", "primitivePlane"}},
		{{EffectType::kGather01}, {"GatherEffect01", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::kScatter01}, {"ScatterEffect01", "./Resources/white.png", "primitiveSphere"}},
	};

	//パーティクル
	std::unordered_map<EffectType, std::unique_ptr<ParticleEmitter>> effectEmitters_;

	ModelPlatform* modelPlatform_ = nullptr;
};

