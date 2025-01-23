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



};