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

	bool IsMouseOverSprite(const Vector2& mousePos, const std::unique_ptr<Sprite>& sprite);



private:

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	uint32_t selectStageNum_ = 1;
	static const uint32_t kMaxStageNum_ = 7;

	//BackGround
	uint32_t background_;
	std::unique_ptr<Sprite> backgroundSprite_;

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
	//bundle
	std::vector<uint32_t> bundleTextures_;
	std::vector<std::unique_ptr<Sprite>> bundleSprites_;
	//selectStage
	std::vector<uint32_t> stageTextures_;
	std::vector<std::unique_ptr<Sprite>> stageSprites_;
	//
	std::vector<uint32_t> difficulty_;
	std::vector<std::unique_ptr<Sprite>> difficultySprites_;


	std::vector<bool> bundleActive_;
	bool selectSpr = true;
};