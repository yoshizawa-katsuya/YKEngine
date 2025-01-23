#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"

class SelectScene : public BaseScene
{
public:

	~SelectScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	bool IsPointInBounds(const Vector2& point, const Vector2& minBounds, const Vector2& maxBounds);

private:

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	//BackGround
	uint32_t background_;
	std::unique_ptr<Sprite> backgroundSprite_;

	//select
	std::vector<uint32_t> selectTextures_;
	std::vector<std::unique_ptr<Sprite>> selectSprites_;
	//bundle
	std::vector<uint32_t> bundleTextures_;
	std::vector<std::unique_ptr<Sprite>> bundleSprites_;



	//
	uint32_t difficulty1_;
	std::unique_ptr<Sprite> difficultySprite1_;
	uint32_t difficulty2_;
	std::unique_ptr<Sprite> difficultySprite2_;
	uint32_t difficulty3_;
	std::unique_ptr<Sprite> difficultySprite3_;


};