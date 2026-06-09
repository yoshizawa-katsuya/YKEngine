#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Camera.h"
#include "Transition.h"
#include "Ui.h"
#include <string>

class ClearScene : public YKEngine::BaseScene
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
	~ClearScene();

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了。
	/// </summary>
	void Finalize();

private:
	//デバイス
	YKEngine::DirectXCommon* dxCommon_;
	YKEngine::Audio* audio_;
	YKEngine::Input* input_;
	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;
	
	// カメラ
	YKEngine::Camera* mainCamera_ = nullptr;
	std::unique_ptr<YKEngine::Camera> camera_;
	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	//クリアの文字のスプライト
	std::unique_ptr<YKEngine::Sprite> clearSprite_;
	//ui
	std::unique_ptr<Ui> ui_;
	// 次のシーン名
	std::string nextSceneName_;
	//遷移演出
	std::unique_ptr<Transition> transition_;

	MenuState menuState_ = MenuState::Retry;

	bool isStartedTransition_ = false;
};

