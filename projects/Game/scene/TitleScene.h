#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"
#include "Transition.h"
#include "Ui.h"
#include "Camera.h"
#include "Player.h"
#include "Difficulty.h"

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
	// タイトル画面のステート
	enum class State {
		START,
		OPTIONS,
		EXIT
	};
	State state_ = State::START; // デフォルトはSTART

	//難易度
	Difficulty difficulty_ = Difficulty::EASY; // デフォルトはEASY

private:

	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Audio* audio_;
	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	YKEngine::Camera* mainCamera_ = nullptr;

	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	std::unique_ptr<YKEngine::Camera> camera_;
	//タイトルスプライト
	std::unique_ptr<YKEngine::Sprite> titleSprite_;
	
	// 画面遷移のクラス
	std::unique_ptr<Transition> transition_;

	//UI
	std::unique_ptr<Ui>ui_;

	// 画面遷移が開始されたかどうかのフラグ
	bool isStartedTransition_ = false;

	std::string nextSceneName_; // 次のシーンの名前

	std::shared_ptr<YKEngine::BaseModel> modelPlayer_;
	//プレイヤー
	std::unique_ptr<Player> demoPLayer1_;
	std::unique_ptr<Player> demoPlayer2_;
};

