#pragma once
#include "Struct.h"
#include "ParticleTypes.h"

class ParticleEmitter
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name = 'name'>パーティクルグループの名前</param>
	/// <param name = 'count'>発生数</param>
	/// <param name = 'frequency'>発生頻度</param>
	ParticleEmitter(const std::string& name, uint32_t count, float frequency);

	void Initialize(uint32_t textureHandle);

	void Update(const Vector4& color = {1.0f, 1.0f, 1.0f, 1.0f});

	void Emit(const Vector4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	void SetTransform(const EulerTransform& transform) { transform_ = transform; }

	void SetTranslation(const Vector3& translation) { transform_.translation = translation; }

	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	void SetRandTranslationMin(const Vector3& translationMin) { randTranslateMin_ = translationMin; }

	void SetRandTranslationMax(const Vector3& translationMax) { randTranslateMax_ = translationMax; }

	void SetIsRandomColor(bool isRandomColor) { randomFlags_.color = isRandomColor; }

	void SetIsRandomTranslate(bool isRandomTranslate) { randomFlags_.translate = isRandomTranslate; }

	void SetIsRandomVelocity(bool isRandomVelocity) { randomFlags_.velocity = isRandomVelocity; }

	void SetIsRandomRotate(bool isRandomRotate) { randomFlags_.rotate = isRandomRotate; }

	void SetIsRandomScele(bool isRandomScale) { randomFlags_.scale = isRandomScale; }

	void SetCount(uint32_t count) { count_ = count; }

	void SetFrequency(float frequency);

	Vector3& GetTranslate() { return transform_.translation; }
	const Vector3& GetTranslate() const { return transform_.translation; }

	Vector3& GetScele() { return transform_.scale; }
	const Vector3& GetScele() const { return transform_.scale; }

	Vector3& GetRandTranslateMin(){ return randTranslateMin_; }
	const Vector3& GetRandTranslateMin() const { return randTranslateMin_; }

	Vector3& GetRandTranslateMax() { return randTranslateMax_; }
	const Vector3& GetRandTranslateMax() const { return randTranslateMax_; }

	float& GetFrequency() { return frequency_; }
	float GetFrequency() const { return frequency_; }

	uint32_t& GetCount() { return count_; }
	uint32_t GetCount() const { return count_; }

	bool& GetIsRandomColor() { return randomFlags_.color; }
	bool GetIsRandomColor() const { return randomFlags_.color; }

	bool& GetIsRandomTranslate() { return randomFlags_.translate; }
	bool GetIsRandomTranslate() const { return randomFlags_.translate; }

	bool& GetIsRandomVelocity() { return randomFlags_.velocity; }
	bool GetIsRandomVelocity() const { return randomFlags_.velocity; }

	bool& GetIsRandomRotate() { return randomFlags_.rotate; }
	bool GetIsRandomRotate() const { return randomFlags_.rotate; }

	bool& GetIsRandomScele() { return randomFlags_.scale; }
	bool GetIsRandomScele() const { return randomFlags_.scale; }

private:

	std::string name_;
	EulerTransform transform_; //!< エミッタのTransform
	uint32_t count_;	//!< 発生数
	float frequency_; //!<　発生頻度
	float frequencyTime_; //!<頻度用時刻
	const float kDeltaTime_ = 1.0f / 60.0f;

	ParticleRandomizationFlags randomFlags_;

	//TODO: 構造体にまとめる 拡縮や回転にも対応させる
	Vector3 randTranslateMin_ = { -1.0f, -1.0f, -1.0f };
	Vector3 randTranslateMax_ = { 1.0f, 1.0f, 1.0f };

};

