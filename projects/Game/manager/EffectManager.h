#pragma once
#include <memory>
#include "ParticleEmitter.h"
class ModelPlatform;
class BaseModel;

enum class EffectType
{
	kHit01,
	kGather01,	//集まるエフェクト1。カラフルな球
	kGather02,	//集まるエフェクト2。カラフルな四角
	kScatter01,	//散らばるエフェクト1。黒い球
	kScatter02,	//散らばるエフェクト2。高速。黄色い球
	kScatter03,	//散らばるエフェクト3。高速。黒い球
	kScatter04,	//散らばるエフェクト4。高速。カラフルな球
	kPlayerBulletTrac01, //弾丸の軌跡エフェクト
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

	/// <summary>
	/// エフェクトの削除。
	/// </summary>
	/// <param name="effectType">エフェクトの種類</param>
	void ClearEffects(EffectType effectType);

	/// <summary>
	/// 全てのエフェクトの削除。
	/// </summary>
	void ClearAllEffects();

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
		{{EffectType::kGather02}, {"GatherEffect02", "./Resources/white.png", "primitiveCube"}},
		{{EffectType::kScatter01}, {"ScatterEffect01", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::kScatter02}, {"ScatterEffect02", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::kScatter03}, {"ScatterEffect03", "./Resources/black.png", "primitiveSphere"}},
		{{EffectType::kScatter04}, {"ScatterEffect04", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::kPlayerBulletTrac01}, {"PlayerBulletTrac01", "./Resources/white.png", "primitiveSphere"}},
	};

	//パーティクル
	std::unordered_map<EffectType, std::unique_ptr<ParticleEmitter>> effectEmitters_;

	ModelPlatform* modelPlatform_ = nullptr;
};

