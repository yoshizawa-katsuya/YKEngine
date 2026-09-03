#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"


// 色の定数
constexpr YKEngine::Vector4 kColorPressKey = { 0.902f, 0.902f, 0.902f, 1.0f }; // #E6E6E6
constexpr float kAlphaStable = 1.0f;
constexpr float kAlphaFlickerMin = 0.3f;
constexpr float kFlickerSpeed = 6.0f; // 点滅の速さ（大きいほど速い）

/// <summary>
/// タイトル画面のクラス。
/// </summary>
class TitleScene : public YKEngine::BaseScene
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TitleScene() override;

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了。
	/// </summary>
	void Finalize() override;

private:

	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Audio* audio_;
	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;
	
	std::unique_ptr<YKEngine::Sprite> titleLINKSprite_;
	std::unique_ptr<YKEngine::Sprite> titleColonSprite_;
	std::unique_ptr<YKEngine::Sprite> titleOVERSprite_;
	YKEngine::Vector2 titlePos_;

	std::unique_ptr<YKEngine::Sprite> startSprite_;
	YKEngine::Vector2 startPos_;

	float pressKeyTimer_ = 0.0f;           // 点滅用の経過時間

	
};

