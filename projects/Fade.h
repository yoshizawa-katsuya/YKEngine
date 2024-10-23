#pragma once
#include "SpritePlatform.h"
#include "Sprite.h"
#include "TextureManager.h"

class Fade {

public:

	enum class Status {
		None,
		FadeIn,
		FadeOut,
	};

	void Initialize();

	void Start(Status status, float duration);

	void Update();

	void Draw();

	bool GetContinue() { return isContinue; }
private:

	//デバイス
	DirectXCommon* dxCommon_;
	SpritePlatform* spritePlatform_;

	uint32_t curtainSprite_ = 0;

	std::unique_ptr<Sprite> sprite_ = nullptr;

	Status status_ = Status::None;

	float duration_ = 0.0f;

	float counter_ = 0.0f;

	bool isContinue = false;
};