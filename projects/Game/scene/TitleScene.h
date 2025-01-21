#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"

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
	
	uint32_t selectStageNum_ = 1;
	static const uint32_t kMaxStageNum_ = 7;

	//select
	uint32_t selectTutorial_;
	std::unique_ptr<Sprite> tutorialSprite_;

	std::array<uint32_t, kMaxStageNum_> selectTextureHandles_;

	enum class positionType {
		kLeftSide,
		kCenter,
		kRightSide,
	};
	std::array<std::unique_ptr<Sprite>, 3> selectSprites_;

	/*
	uint32_t select1_;
	std::unique_ptr<Sprite> selectdSprite1_;
	uint32_t select2_;
	std::unique_ptr<Sprite> selectdSprite2_;
	uint32_t select3_;
	std::unique_ptr<Sprite> selectdSprite3_;
	uint32_t select4_;
	std::unique_ptr<Sprite> selectdSprite4_;
	uint32_t select5_;
	std::unique_ptr<Sprite> selectdSprite5_;
	uint32_t select6_;
	std::unique_ptr<Sprite> selectdSprite6_;
	uint32_t select7_;
	std::unique_ptr<Sprite> selectdSprite7_;
	*/

};

