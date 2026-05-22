#include "Effect.h"

#include "TextureManager.h"
#include "ModelPlatform.h"
#include "Input.h"

#include <numbers>

using namespace YKEngine;

void Effect::Initialize()
{
	//紙吹雪テクスチャ
	uint32_t confettiTexture =
		TextureManager::GetInstance()->Load(
			"Resources/particle/confetti.png"
		);

	//板ポリモデル
	std::shared_ptr<BaseModel> model =
		ModelPlatform::GetInstance()->CreateRigidModel(
			"./resources/particle", "plane.obj");

	//Emitter生成
	confettiEmitter_ =
		std::make_unique<ParticleEmitter>("Confetti");

	confettiEmitter_->Initialize(confettiTexture, model);

	//発生位置(画面上部)
	confettiEmitter_->SetTranslation({ 0.0f, 8.0f, 0.0f });

	//発生数
	confettiEmitter_->SetCount(20);

	//発生頻度
	confettiEmitter_->SetFrequency(0.03f);

	//Velocityランダム
	confettiEmitter_->SetIsRandomVelocity(true);

	confettiEmitter_->SetRandVelocityMin({
		-1.5f,
		-3.0f,
		-1.5f
		});

	confettiEmitter_->SetRandVelocityMax({
		1.5f,
		-1.0f,
		1.5f
		});

	//回転ランダム
	confettiEmitter_->SetIsRandomRotate(true);

	confettiEmitter_->SetRandRotateMin({
		-static_cast<float>(std::numbers::pi),
		-static_cast<float>(std::numbers::pi),
		-static_cast<float>(std::numbers::pi)
		});

	confettiEmitter_->SetRandRotateMax({
		static_cast<float>(std::numbers::pi),
		static_cast<float>(std::numbers::pi),
		static_cast<float>(std::numbers::pi)
		});

	//回転速度ランダム
	confettiEmitter_->SetIsRandomRotationVelocity(true);

	confettiEmitter_->SetRandRotationVelocityMin({
		-0.3f,
		-0.3f,
		-0.3f
		});

	confettiEmitter_->SetRandRotationVelocityMax({
		0.3f,
		0.3f,
		0.3f
		});

	//Scaleランダム
	confettiEmitter_->SetIsRandomScele(true);

	confettiEmitter_->SetRandScaleMin({
		0.02f,
		0.008f,
		0.001f
		});

	confettiEmitter_->SetRandScaleMax({
		0.05f,
		0.015f,
		0.001f
		});

	//Lifetimeランダム
	confettiEmitter_->SetIsRandomLifeTime(true);

	confettiEmitter_->SetRandLifeTimeMin(2.0f);
	confettiEmitter_->SetRandLifeTimeMax(5.0f);

	//ランダムカラー
	confettiEmitter_->SetIsRandomColor(true);

	//Billboard OFF
	confettiEmitter_->SetIsUseBillboard(false);

	//減速
	confettiEmitter_->SetIsDownVelocity(true);

	//通常ブレンド
	confettiEmitter_->SetDrawMode(
		ParticleDrawMode::kNormalBlend
	);
}

void Effect::Update()
{
#ifdef _DEBUG

	//SPACEで再生
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		StartConfetti();
	}

#endif

	//再生中
	if (isPlayConfetti_)
	{
		confettiTimer_ += 1.0f / 60.0f;

		//Emitter更新
		confettiEmitter_->Update();

		//一定時間で終了
		if (confettiTimer_ >= kConfettiDuration_)
		{
			isPlayConfetti_ = false;
			confettiTimer_ = 0.0f;
		}
	}
}

void Effect::Draw()
{
	//ParticleManager側で描画するため空
}

void Effect::StartConfetti()
{
	isPlayConfetti_ = true;

	confettiTimer_ = 0.0f;
}