#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"

class GameClearScene : public BaseScene
{
public:
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
};

