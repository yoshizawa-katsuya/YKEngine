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

	//シングルトンインスタンスの取得
	static EffectManager* GetInstance();

	//初期化
	void Initialize();

	//更新
	void Update();

	//ヒットエフェクトの生成
	void SpawnHitEffect(const Vector3& position);

private:

	EffectManager() = default;
	~EffectManager() = default;
	EffectManager(EffectManager&) = delete;
	const EffectManager& operator=(EffectManager&) = delete;

	//パーティクル
	std::unique_ptr<ParticleEmitter> emitter_;

	ModelPlatform* modelPlatform_ = nullptr;
};

