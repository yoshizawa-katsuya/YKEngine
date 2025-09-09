#pragma once
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"
#include <array>
#include <memory>
#include <cmath>

class PauseMenu
{
public:
	void Initialize();
	void Update();
	void Draw();
private:
	double easeInOutCirc(double x);
private:
	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;
	std::array<std::unique_ptr<Sprite>, 4>sprites_;
	std::array<std::string, 4>texturePaths;
	std::array<Vector2, 4>positions;
	std::array<Vector2,4>sizes;
	bool isPaused_ = false;//ポーズ中か
	float easeTimer_ = 0.0f;
	const float easeSpeed = 0.04f;//遷移速度

};

