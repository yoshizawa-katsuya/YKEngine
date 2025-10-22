#include "ParticleEmitter.h"
#include "ParticleManager.h"

ParticleEmitter::ParticleEmitter(const std::string& name)
{

	name_ = name;

	frequencyTime_ = 0.0f;

	transform_.translation = { 0.0f, 0.0f, 0.0f };
	transform_.rotation = { 0.0f, 0.0f, 0.0f };
	transform_.scale = { 1.0f, 1.0f, 1.0f };

	behavior_ = std::make_shared<ParticleBehavior>();

	InitializeGlobalVariables();
}

void ParticleEmitter::Initialize(uint32_t textureHandle, std::shared_ptr<BaseModel> model)
{

	ParticleManager::GetInstance()->CreateParticleGroup(name_, textureHandle, model, behavior_);

}

void ParticleEmitter::Update()
{
	frequencyTime_ += kDeltaTime_;	//時刻を進める
	if (frequency_ <= frequencyTime_) {	//頻度より大きいなら発生
		ParticleManager::GetInstance()->Emit(name_, transform_, count_, randomFlags_,
			color_, rangeParams_);	//発生処理
		frequencyTime_ -= frequency_;	//余計に過ぎた時間も加味して頻度計算する
	}
}

void ParticleEmitter::Emit()
{

	ParticleManager::GetInstance()->Emit(name_, transform_, count_, randomFlags_,
		color_, rangeParams_);	//発生処理

}

void ParticleEmitter::ApplyGlobalVariables()
{

	transform_.translation = globalVariables_->GetVector3Value(name_, "translation");
	transform_.rotation = globalVariables_->GetVector3Value(name_, "rotation");
	transform_.scale = globalVariables_->GetVector3Value(name_, "scale");
	count_ = static_cast<uint32_t>(globalVariables_->GetIntValue(name_, "count"));
	frequency_ = globalVariables_->GetFloatValue(name_, "frequency");
	randomFlags_.color = globalVariables_->GetBoolValue(name_, "randomFlagsColor");
	randomFlags_.translate = globalVariables_->GetBoolValue(name_, "randomFlagsTranslation");
	randomFlags_.rotate = globalVariables_->GetBoolValue(name_, "randomFlagsRotation");
	randomFlags_.scale = globalVariables_->GetBoolValue(name_, "randomFlagsScale");
	randomFlags_.velocity = globalVariables_->GetBoolValue(name_, "randomFlagsVelocity");
	randomFlags_.lifeTime = globalVariables_->GetBoolValue(name_, "randomFlagsLifeTime");
	rangeParams_.translate.max = globalVariables_->GetVector3Value(name_, "rangeParamsTranslateMax");
	rangeParams_.translate.min = globalVariables_->GetVector3Value(name_, "rangeParamsTranslateMin");
	rangeParams_.scale.max = globalVariables_->GetVector3Value(name_, "rangeParamsScaleMax");
	rangeParams_.scale.min = globalVariables_->GetVector3Value(name_, "rangeParamsScaleMin");
	rangeParams_.rotate.max = globalVariables_->GetVector3Value(name_, "rangeParamsRotateMax");
	rangeParams_.rotate.min = globalVariables_->GetVector3Value(name_, "rangeParamsRotateMin");
	rangeParams_.velocity.max = globalVariables_->GetVector3Value(name_, "rangeParamsVelocityMax");
	rangeParams_.velocity.min = globalVariables_->GetVector3Value(name_, "rangeParamsVelocityMin");
	rangeParams_.lifeTime.max = globalVariables_->GetFloatValue(name_, "rangeParamsLifeTimeMax");
	rangeParams_.lifeTime.min = globalVariables_->GetFloatValue(name_, "rangeParamsLifeTimeMin");
	behavior_->isFaceToVelocityDirection = globalVariables_->GetBoolValue(name_, "behaviorIsFaceToVelocityDirection");
	behavior_->isConstantVelocity = globalVariables_->GetBoolValue(name_, "behaviorIsConstantVelocity");
	behavior_->speed = globalVariables_->GetFloatValue(name_, "behaviorSpeed");
	behavior_->isScaleToDisappear = globalVariables_->GetBoolValue(name_, "bahaviorIsSceleToDisappear");
	behavior_->isScaleToAppear = globalVariables_->GetBoolValue(name_, "behaviorIsScaleToAppear");
	behavior_->easingTypeForScale = static_cast<EasingType>(globalVariables_->GetIntValue(name_, "bahaviorEasingTypeForScale"));
	behavior_->isUseBillboard = globalVariables_->GetBoolValue(name_, "behaviorIsUseBillboard");
	behavior_->isdownVelocity = globalVariables_->GetBoolValue(name_, "behaviorIsDownVelocity");
	behavior_->isfixedDistance = globalVariables_->GetBoolValue(name_, "behaviorIsFixedDistance");
	behavior_->distance = globalVariables_->GetFloatValue(name_, "behaviorDistance");
	behavior_->isHeadCenter = globalVariables_->GetBoolValue(name_, "behaviorIsHeadCenter");
	behavior_->isTimeFadeOut = globalVariables_->GetBoolValue(name_, "behaviorIsTimeFadeOut");
	color_ = globalVariables_->GetColorValue(name_, "color");


}

void ParticleEmitter::SetFrequency(float frequency)
{

	frequency_ = frequency;
	frequencyTime_ = 0.0f;

}

void ParticleEmitter::InitializeGlobalVariables()
{
	//グローバル変数登録
	globalVariables_->CreateGroup(name_);
	globalVariables_->AddItem(name_, "translation", transform_.translation);
	globalVariables_->AddItem(name_, "rotation", transform_.rotation);
	globalVariables_->AddItem(name_, "scale", transform_.scale);
	globalVariables_->AddItem(name_, "count", static_cast<int32_t>(count_));
	globalVariables_->AddItem(name_, "frequency", frequency_);
	globalVariables_->AddItem(name_, "randomFlagsColor", randomFlags_.color);
	globalVariables_->AddItem(name_, "randomFlagsTranslation", randomFlags_.translate);
	globalVariables_->AddItem(name_, "randomFlagsRotation", randomFlags_.rotate);
	globalVariables_->AddItem(name_, "randomFlagsScale", randomFlags_.scale);
	globalVariables_->AddItem(name_, "randomFlagsVelocity", randomFlags_.velocity);
	globalVariables_->AddItem(name_, "randomFlagsLifeTime", randomFlags_.lifeTime);
	globalVariables_->AddItem(name_, "rangeParamsTranslateMax", rangeParams_.translate.max);
	globalVariables_->AddItem(name_, "rangeParamsTranslateMin", rangeParams_.translate.min);
	globalVariables_->AddItem(name_, "rangeParamsScaleMax", rangeParams_.scale.max);
	globalVariables_->AddItem(name_, "rangeParamsScaleMin", rangeParams_.scale.min);
	globalVariables_->AddItem(name_, "rangeParamsRotateMax", rangeParams_.rotate.max);
	globalVariables_->AddItem(name_, "rangeParamsRotateMin", rangeParams_.rotate.min);
	globalVariables_->AddItem(name_, "rangeParamsVelocityMax", rangeParams_.velocity.max);
	globalVariables_->AddItem(name_, "rangeParamsVelocityMin", rangeParams_.velocity.min);
	globalVariables_->AddItem(name_, "rangeParamsLifeTimeMax", rangeParams_.lifeTime.max);
	globalVariables_->AddItem(name_, "rangeParamsLifeTimeMin", rangeParams_.lifeTime.min);
	globalVariables_->AddItem(name_, "behaviorIsFaceToVelocityDirection", behavior_->isFaceToVelocityDirection);
	globalVariables_->AddItem(name_, "behaviorIsConstantVelocity", behavior_->isConstantVelocity);
	globalVariables_->AddItem(name_, "behaviorSpeed", behavior_->speed);
	globalVariables_->AddItem(name_, "bahaviorIsSceleToDisappear", behavior_->isScaleToDisappear);
	globalVariables_->AddItem(name_, "behaviorIsScaleToAppear", behavior_->isScaleToAppear);
	globalVariables_->AddItem(name_, "bahaviorEasingTypeForScale", static_cast<int32_t>(behavior_->easingTypeForScale));
	globalVariables_->AddItem(name_, "behaviorIsUseBillboard", behavior_->isUseBillboard);
	globalVariables_->AddItem(name_, "behaviorIsDownVelocity", behavior_->isdownVelocity);
	globalVariables_->AddItem(name_, "behaviorIsFixedDistance", behavior_->isfixedDistance);
	globalVariables_->AddItem(name_, "behaviorDistance", behavior_->distance);
	globalVariables_->AddItem(name_, "behaviorIsHeadCenter", behavior_->isHeadCenter);
	globalVariables_->AddItem(name_, "behaviorIsTimeFadeOut", behavior_->isTimeFadeOut);
	globalVariables_->AddItem(name_, "color", color_);

	ApplyGlobalVariables();	//初期値を反映
}
