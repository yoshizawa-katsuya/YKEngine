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

	void UpdateStart();

	void UpdateMain();

	void UpdateEnd();

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	//シーンのフェーズ
	enum class Phase {
		kStart,	//開始部
		kMain,	//メイン部
		kEnd,	//終了部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kStart;

	std::unique_ptr<Fade> fade_;

	std::array<std::unique_ptr<Sprite>, 8> gameOvers_;

	int menuState_ = 0;
	int frameCount_ = 0;
	int blinkIndex_ = 0;

	LoopSoundData menuSE_;
	LoopSoundData ketteiSE_;
};

