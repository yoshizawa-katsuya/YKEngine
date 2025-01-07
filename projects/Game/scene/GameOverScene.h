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

class GameOverScene : public BaseScene
{
public:

	~GameOverScene() override;

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

	uint32_t textureHandle_;

	std::unique_ptr<Sprite> sprite_;
};