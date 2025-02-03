#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"
#include "SelectScene.h"

class SelectStage : public BaseScene
{
public:

	~SelectStage() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	bool IsMouseOverSprite(const Vector2& mousePos, const std::unique_ptr<Sprite>& sprite);

	uint32_t GetSelectedStage() const;

private:
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	//BackGround
	uint32_t background_;
	std::unique_ptr<Sprite> backgroundSprite_;

	//bundle 7
	std::vector<bool> bundleActive_;
	std::vector<uint32_t> bundleTextures_;
	std::vector<std::unique_ptr<Sprite>> bundleSprites_;

	//selectStage 10
	std::vector<uint32_t> stageTextures_;
	std::vector<std::unique_ptr<Sprite>> stageSprites_;

	//return Button
	uint32_t reButton1_;
	std::unique_ptr<Sprite> reButtonSprite1_;
	uint32_t reButton2_;
	std::unique_ptr<Sprite> reButtonSprite2_;
	uint32_t reButton3_;
	std::unique_ptr<Sprite> reButtonSprite3_;
	


	//
	SelectScene* selectScene_ = nullptr;
	uint32_t selectedBundle_ = 0;
	uint32_t selectedStage_ = 0;

	bool set = false;

};

