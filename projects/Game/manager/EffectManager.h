#pragma once
#include <memory>
#include "ParticleEmitter.h"
class ModelPlatform;

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
	/// ヒットエフェクトの生成。
	/// </summary>
	/// <param name="position">生成位置</param>
	void SpawnHitEffect(const Vector3& position);

	/// <summary>
	/// プレイヤー開始エフェクトの生成。
	/// </summary>
	/// <param name="position">生成位置</param>
	void SpawnPlayerStartEffect(const Vector3& position);

private:

	EffectManager() = default;
	~EffectManager() = default;
	EffectManager(EffectManager&) = delete;
	const EffectManager& operator=(EffectManager&) = delete;

	//パーティクル
	std::unique_ptr<ParticleEmitter> emitter_;
	std::unique_ptr<ParticleEmitter> playerSpawnEmitter_;

	ModelPlatform* modelPlatform_ = nullptr;
};

