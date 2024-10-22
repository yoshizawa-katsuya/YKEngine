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

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	//シーンのフェーズ
	enum class Phase {
		kFadeIn,	//フェードイン
		kMain,	//メイン部
		kFadoOut,	//フェードアウト
	};

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	//uint32_t textureHandle_;

	//std::unique_ptr<Sprite> sprite_;

	std::unique_ptr<Fade> fade_ = nullptr;

	//現在のフェーズ
	Phase phase_ = Phase::kFadeIn;
};

