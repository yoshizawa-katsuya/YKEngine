#pragma once
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"
#include "Fade.h"
#include <array>
#include <memory>
#include <cmath>

class PauseMenu
{
public:
	void Initialize();
	void Update();
	void Draw(bool canDrawUI);
	void UpdateMenu();
public:
	bool IsPaused();//ポーズ中かを取得

	void SetMenuSE(LoopSoundData* menuSE) { menuSE_ = menuSE; };//メニューSEセット

	void SetKetteiSE(LoopSoundData* ketteiSE) { ketteiSE_ = ketteiSE; };//決定SEセット

private:
	double easeInOutCirc(double x);
private:
	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;
	std::array<std::unique_ptr<Sprite>, 7>sprites_;
	std::array<std::string, 7>texturePaths;
	std::array<Vector2, 7>positions;
	std::array<Vector2,7>sizes;
	std::unique_ptr<Fade>fade_;
	std::string nextScene_;
	bool isPaused_ = false;//ポーズ中か
	bool fadeStart_ = false;//フェード開始フラグ
	bool isControlScreen_ = false;//操作方法画面表示しているか
	bool controlClosing_ = false;
	int menuState = 1;
	float easeTimer_ = 0.0f;
	float cursorTimer_ = 0.0f;//選択中演出タイマー
	float controlEaseTimer_ = 0.0f;//操作方法画面イージング
	float cursorSpeed_ = 3.0f;//演出速度
	float cursorAmplitude_ = 0.1f;//拡大幅
	const float easeSpeed = 0.04f;//遷移速度

	LoopSoundData* menuSE_ = nullptr;
	LoopSoundData* ketteiSE_ = nullptr;
};

