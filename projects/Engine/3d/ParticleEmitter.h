#pragma once
#include "Struct.h"

class ParticleEmitter
{
public:

	ParticleEmitter(const std::string& name, uint32_t count, float frequency);

	void Initialize(uint32_t textureHandle);

	void Update();

	void Emit();

private:

	std::string name_;
	Transforms transform_; //!< エミッタのTransform
	uint32_t count_;	//!< 発生数
	float frequency_; //!<　発生頻度
	float frequencyTime_; //!<頻度用時刻
	const float kDeltaTime_ = 1.0f / 60.0f;

};

