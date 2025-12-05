#pragma once
#include "Struct.h"
#include "ParticleTypes.h"
#include <memory>
#include "BaseModel.h"
#include "GlobalVariables.h"
#include "Color.h"

namespace YKEngine
{

/// <summary>
/// パーティクル生成クラス。
/// パーティクルの発生数、発生頻度、ランダム化の有無、挙動などを管理する。
/// GlobalVariablesクラスを通して、パーティクルエディターとしての役割も担う。
/// パーティクルの実体はParticleManagerクラスが管理する。
/// </summary>
class ParticleEmitter
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name = 'name'>パーティクルグループの名前</param>
	ParticleEmitter(const std::string& name);

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="model">モデル</param>
	void Initialize(uint32_t textureHandle, std::shared_ptr<BaseModel> model);

	/// <summary>
	/// 更新。
	/// 時間経過で発生頻度に達したらパーティクルを発生させる。
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクルを発生させる。
	/// </summary>
	void Emit();

	/// <summary>
	/// グローバル変数を適用する。
	/// </summary>
	void ApplyGlobalVariables();

	void SetTransform(const EulerTransform& transform) { transform_ = transform; }

	void SetTranslation(const Vector3& translation) { transform_.translation = translation; }

	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	void SetRandTranslationMin(const Vector3& translationMin) { rangeParams_.translate.min = translationMin; }

	void SetRandTranslationMax(const Vector3& translationMax) { rangeParams_.translate.max = translationMax; }

	void SetRandRotateMin(const Vector3& rotateMin) { rangeParams_.rotate.min = rotateMin; }

	void SetRandRotateMax(const Vector3& rotateMax) { rangeParams_.rotate.max = rotateMax; }

	void SetRandScaleMin(const Vector3& scaleMin) { rangeParams_.scale.min = scaleMin; }

	void SetRandScaleMax(const Vector3& scaleMax) { rangeParams_.scale.max = scaleMax; }

	void SetRandVelocityMin(const Vector3& velocityMin) { rangeParams_.velocity.min = velocityMin; }

	void SetRandVelocityMax(const Vector3& velocityMax) { rangeParams_.velocity.max = velocityMax; }

	void SetRandLifeTimeMin(float lifeTimeMin) { rangeParams_.lifeTime.min = lifeTimeMin; }

	void SetRandLifeTimeMax(float lifeTimeMax) { rangeParams_.lifeTime.max = lifeTimeMax; }

	void SetIsRandomColor(bool isRandomColor) { randomFlags_.color = isRandomColor; }

	void SetIsRandomTranslate(bool isRandomTranslate) { randomFlags_.translate = isRandomTranslate; }

	void SetIsRandomVelocity(bool isRandomVelocity) { randomFlags_.velocity = isRandomVelocity; }

	void SetIsRandomRotate(bool isRandomRotate) { randomFlags_.rotate = isRandomRotate; }

	void SetIsRandomScele(bool isRandomScale) { randomFlags_.scale = isRandomScale; }

	void SetIsRandomLifeTime(bool isRandomLifeTime) { randomFlags_.lifeTime = isRandomLifeTime; }

	void SetIsFaceToVelocityDirection(bool isFaceToVelocityDirection) { behavior_->isFaceToVelocityDirection = isFaceToVelocityDirection; }

	void SetIsConstantVelocity(bool isConstantVelocity) { behavior_->isConstantVelocity = isConstantVelocity; }

	void SetEasingTypeForScale(EasingType easingType) { behavior_->easingTypeForScale = easingType; }

	void SetSpeed(float speed) { behavior_->speed = speed; }

	void SetIsScaleToDisAppear(bool isScaleToDisappear) { behavior_->isScaleToDisappear = isScaleToDisappear; }

	void SetIsScaleToAppear(bool isScaleToAppear) { behavior_->isScaleToAppear = isScaleToAppear; }

	void SetIsUseBillboard(bool isUseBillboard) { behavior_->isUseBillboard = isUseBillboard; }

	void SetIsDownVelocity(bool isDownVelocity) { behavior_->isdownVelocity = isDownVelocity; }

	void SetCount(uint32_t count) { count_ = count; }

	/// <summary>
	/// 発生頻度の設定。
	/// タイマーをリセットする。
	/// </summary>
	/// <param name="frequency">発生頻度</param>
	void SetFrequency(float frequency);

	Vector3& GetTranslate() { return transform_.translation; }
	const Vector3& GetTranslate() const { return transform_.translation; }

	Vector3& GetScele() { return transform_.scale; }
	const Vector3& GetScele() const { return transform_.scale; }

	Vector3& GetRandTranslateMin(){ return rangeParams_.translate.min; }
	const Vector3& GetRandTranslateMin() const { return rangeParams_.translate.min; }

	Vector3& GetRandTranslateMax() { return rangeParams_.translate.max; }
	const Vector3& GetRandTranslateMax() const { return rangeParams_.translate.max; }

	Vector3& GetRandRotateMin() { return rangeParams_.rotate.min; }
	const Vector3& GetRandRotateMin() const { return rangeParams_.rotate.min; }

	Vector3& GetRandRotateMax() { return rangeParams_.rotate.max; }
	const Vector3& GetRandRotateMax() const { return rangeParams_.rotate.max; }

	Vector3& GetRandScaleMin() { return rangeParams_.scale.min; }
	const Vector3& GetRandScaleMin() const { return rangeParams_.scale.min; }

	Vector3& GetRandScaleMax() { return rangeParams_.scale.max; }
	const Vector3& GetRandScaleMax() const { return rangeParams_.scale.max; }

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

	/// <summary>
	/// グローバル変数の初期化。
	/// </summary>
	void InitializeGlobalVariables();

	GlobalVariables* globalVariables_ = GlobalVariables::GetInstance();	

	std::string name_;
	EulerTransform transform_; //!< エミッタのTransform
	uint32_t count_ = 3;	//!< 発生数
	float frequency_ = 1.0f; //!<　発生頻度
	float frequencyTime_; //!<頻度用時刻
	const float kDeltaTime_ = 1.0f / 60.0f;
	Color color_ = { 1.0f, 1.0f, 1.0f, 1.0f }; //!< 色

	ParticleRandomizationFlags randomFlags_;

	std::shared_ptr<ParticleBehavior> behavior_;

	//ランダム化の上限下限を管理
	EmitterRangeParams rangeParams_;

};

} // namespace YKEngine