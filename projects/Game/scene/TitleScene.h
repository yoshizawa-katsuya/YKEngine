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
	//==================================================
	// アニメーション用関数
	//==================================================

	void UpdateTitleAnimation();

	// イージング
	float EaseOutCubic(float t);

	float EaseInOutSine(float t);
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
	YKEngine::Vector2 colonPos_;

	std::unique_ptr<YKEngine::Sprite> startSprite_;
	YKEngine::Vector2 startPos_;

	float pressKeyTimer_ = 0.0f;           // 点滅用の経過時間

	//==================================================
	// アニメーション
	//==================================================

	enum class TitleAnimationState
	{
		kOpening,	// 左右から中央へ
		kMoving,	// 左右に往復
	};

	TitleAnimationState animationState_ = TitleAnimationState::kOpening;

	float animationTimer_ = 0.0f;

	// 開始時のアニメーション時間
	static constexpr float kOpeningDuration = 1.0f;

	// 左右への移動幅
	static constexpr float kMoveDistance = 70.0f;

	// 左右への往復にかかる時間
	static constexpr float kMoveDuration = 1.5f;

	// Colonの回転角度
	static constexpr float kColonMaxRotation = 1.57f;

	// Colonの色
	static constexpr YKEngine::Vector4 kColonColorRed =
	{ 1.0f, 0.36f, 0.54f, 1.0f };

	static constexpr YKEngine::Vector4 kColonColorCyan =
	{ 0.24f, 0.87f, 1.0f, 1.0f };

	static constexpr YKEngine::Vector4 kColonColorGreen =
	{ 0.3f, 1.0f, 0.4f, 1.0f };

	// 最初の色変化だけ赤から始める
	bool isFirstColorAnimation_ = true;
};

