#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"
#include "Fade.h"

class TitleScene : public BaseScene
{
public:

	~TitleScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;
	
	//シーンのフェーズ
	enum class Phase {
		kFadeIn,	//フェードイン
		kMain,	//メイン部
		kFadoOut,	//フェードアウト
	};

	//現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	std::unique_ptr<Fade> fade_;

	static uint32_t selectNum_;

	uint32_t textureHandleTitle_;
	uint32_t texturehandleCursor_;

	std::unique_ptr<Sprite> spriteTitle_;
	std::unique_ptr<Sprite> spriteCursor_;

	LoopSoundData bgm1_;

	SoundData SelectSE1_;
	SoundData DecideSE1_;
};

