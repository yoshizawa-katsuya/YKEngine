#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"

/// <summary>
/// ゲームオーバーシーンのクラス
/// </summary>
class GameOverScene : public YKEngine::BaseScene
{
	public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~GameOverScene() override;
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
	//テクスチャハンドル
	uint32_t textureHandle_;
	//スプライト
	std::unique_ptr<YKEngine::Sprite> sprite_;
};

