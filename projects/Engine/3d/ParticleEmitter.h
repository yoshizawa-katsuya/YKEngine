#pragma once
#include "Struct.h"

class ParticleEmitter
{
public:

	ParticleEmitter(const std::string& name, uint32_t count, float frequency);

	void Initialize(uint32_t textureHandle);

	void Update();

	void Update(const Vector4& color);

	void Emit();

	void Emit(const Vector4& color);

	void SetTransform(const EulerTransform& transform) { transform_ = transform; }

	void SetTranslation(const Vector3& translation) { transform_.translation = translation; }

	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	void SetIsRandomColor(bool isRandomColor) { isRandomColor_ = isRandomColor; }

	void SetCount(uint32_t count) { count_ = count; }

	void SetFrequency(float frequency);

private:

	std::string name_;
	EulerTransform transform_; //!< エミッタのTransform
	uint32_t count_;	//!< 発生数
	float frequency_; //!<　発生頻度
	float frequencyTime_; //!<頻度用時刻
	const float kDeltaTime_ = 1.0f / 60.0f;

	bool isRandomColor_ = false;

	Vector3 randTranslateMin_ = { -1.0f, -1.0f, -1.0f };
	Vector3 randTranslateMax_ = { 1.0f, 1.0f, 1.0f };

};

