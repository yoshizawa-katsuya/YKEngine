#pragma once
#include "BaseScene.h"
#include "AnimatedSprite.h"
#include "GameOverSceneStateContext.h"
#include "StateMachine.hpp"
class SceneChangeStaging;
class AudioManager;

namespace YKEngine
{
	class Input;
	class ModelPlatform;
}

/// <summary>
/// ゲームオーバー画面を管理するクラス。
/// ゲームオーバー画面の初期化、更新、描画、終了処理を行う。
/// </summary>
class GameOverScene : public YKEngine::BaseScene, private GameOverSceneStateContext
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
	/// 終了処理。
	/// </summary>
	void Finalize() override;

private:

	/// <summary>
	/// 終了部の更新。
	/// </summary>
	void UpdateEnd() override;

	/// <summary>
	/// シーン遷移演出が終了していたらtrueを返す。
	/// </summary>
	bool IsSceneStagingEnd() const override;

	/// <summary>
	/// シーンが終了するときの遷移演出の開始。
	/// </summary>
	void BeginSceneEndStaging() override;

	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	std::unique_ptr<YKEngine::Sprite> spriteBackGround_;

	SceneChangeStaging* sceneChangeStaging_ = nullptr;//シーンチェンジ演出

	//ステートマシン
	std::unique_ptr<YKEngine::StateMachine<GameOverSceneStateContext>> stateMachine_;

	//オーディオマネージャー
	AudioManager* audioManager_;
};

