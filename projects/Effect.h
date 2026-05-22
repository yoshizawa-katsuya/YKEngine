#pragma once
#include "ParticleEmitter.h"
#include <memory>

class Effect
{
public:

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	//紙吹雪開始
	void StartConfetti();

private:

	//紙吹雪Emitter
	std::unique_ptr<YKEngine::ParticleEmitter> confettiEmitter_;

	//再生中か
	bool isPlayConfetti_ = false;

	//演出タイマー
	float confettiTimer_ = 0.0f;

	//演出時間
	const float kConfettiDuration_ = 3.0f;
};