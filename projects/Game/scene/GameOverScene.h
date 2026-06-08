#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"
#include "Camera.h"
#include "Player.h"
#include "Transition.h"
#include "Ui.h"
#include <string>

/// <summary>
/// ゲームオーバーシーンのクラス
/// </summary>
class GameOverScene : public YKEngine::BaseScene
{
public:
	enum class MenuState
	{
		Retry,
		Title
	};

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
	std::unique_ptr<YKEngine::Sprite> backgroundSprite_;
	// カメラ
	YKEngine::Camera* mainCamera_ = nullptr;
	std::unique_ptr<YKEngine::Camera> camera_;
	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	std::shared_ptr<YKEngine::BaseModel> modelPlayer_;
	//プレイヤー
	std::unique_ptr<Player> player_;
	//遷移演出
	std::unique_ptr<Transition> transition_;
	//UI
	std::unique_ptr<Ui>ui_;

	std::string nextSceneName_;

	MenuState menuState_ = MenuState::Retry;

	bool isStartedTransition_ = false;
};