#pragma once
#include <memory>
#include "ParticleEmitter.h"
class ModelPlatform;

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

