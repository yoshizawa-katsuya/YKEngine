#pragma once
#include <memory>
#include "ParticleEmitter.h"
class YKEngine::ModelPlatform;
class YKEngine::BaseModel;

enum class EffectType
{
	kHit01,
	kHit02,	//黒い球
	kPlayerStart01,	//カラフルな球
	kPlayerStart02,	//カラフルな四角
	kPlayerStart03,	//高速。黄色い球
	kPlayerEnd01,	//高速。黒い球
	kEnemyEnd01,	//高速。カラフルな球
	kPlayerBulletTrac01, //弾丸の軌跡エフェクト
	kConfetti01,	//紙吹雪エフェクト1。カラフルな四角板
	kEnemyBrowAway01, //敵が吹き飛ぶエフェクト1。
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
	void SpawnEffect(EffectType effectType, const YKEngine::Vector3& position);

	/// <summary>
	/// エフェクトの生成。
	/// </summary>
	/// <param name="effectType">エフェクトの種類</param>
	/// <param name="position">生成位置</param>
	/// <param name="count">生成数</param>
	void SpawnEffect(EffectType effectType, const YKEngine::Vector3& position, uint32_t count);

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

	EffectManager() = default;
	~EffectManager() = default;
	EffectManager(EffectManager&) = delete;
	const EffectManager& operator=(EffectManager&) = delete;

	std::shared_ptr<YKEngine::BaseModel> LoadEffectModel(const std::string& modelName, uint32_t textureHnadle);

	struct EffectData
	{
		std::string name;
		std::string textureFilePath;
		std::string modelName;
	};

	std::unordered_map<EffectType, EffectData> effectDatas_
	{
		{{EffectType::kHit01}, {"HitEffect01", "./Resources/circle2.png", "primitivePlane"}},
		{{EffectType::kHit02}, {"HitEffect02", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::kPlayerStart01}, {"PlayerStartEffect01", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::kPlayerStart02}, {"PlayerStartEffect02", "./Resources/white.png", "primitiveCube"}},
		{{EffectType::kPlayerStart03}, {"PlayerStartEffect03", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::kPlayerEnd01}, {"PlayerEndEffect01", "./Resources/black.png", "primitiveSphere"}},
		{{EffectType::kEnemyEnd01}, {"EnemyEndEffect01", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::kPlayerBulletTrac01}, {"PlayerBulletTrac01", "./Resources/white.png", "primitiveSphere"}},
		{{EffectType::kConfetti01}, {"ConfettiEffect01", "./Resources/white.png", "primitivePlane"}},
		{{EffectType::kEnemyBrowAway01}, {"EnemyBrowAway01", "./Resources/white.png", "primitiveSphere"}},
	};

	//パーティクル
	std::unordered_map<EffectType, std::unique_ptr<YKEngine::ParticleEmitter>> effectEmitters_;

	YKEngine::ModelPlatform* modelPlatform_ = nullptr;
};

