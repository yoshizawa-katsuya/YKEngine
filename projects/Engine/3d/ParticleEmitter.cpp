#include "ParticleEmitter.h"
#include "ParticleManager.h"

ParticleEmitter::ParticleEmitter(const std::string& name, uint32_t count, float frequency)
{

	name_ = name;
	count_ = count;
	frequency_ = frequency;

	frequencyTime_ = 0.0f;

	transform_.translate = { 0.0f, 0.0f, 0.0f };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.scale = { 1.0f, 1.0f, 1.0f };

}

void ParticleEmitter::Initialize(uint32_t textureHandle)
{

	ParticleManager::GetInstance()->CreateParticleGroup(name_, textureHandle);

}

void ParticleEmitter::Update()
{

	frequencyTime_ += kDeltaTime_;	//時刻を進める
	if (frequency_ <= frequencyTime_) {	//頻度より大きいなら発生
		ParticleManager::GetInstance()->Emit(name_, transform_.translate, count_);	//発生処理
		frequencyTime_ -= frequency_;	//余計に過ぎた時間も加味して頻度計算する
	}

}

void ParticleEmitter::Emit()
{

	ParticleManager::GetInstance()->Emit(name_, transform_.translate, count_);	//発生処理

}
