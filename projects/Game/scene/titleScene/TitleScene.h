#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "AnimatedSprite.h"
#include "My3dObject.h"
#include "RailMover.h"
#include "InstancingObjects.h"
#include "manager/CameraManager.h"
#include "StageObjects.h"
#include "StateMachine.hpp"
#include "TitleSceneStateContext.h"
#include "TitleUI.h"
class SceneChangeStaging;
class AudioManager;

/// <summary>
/// タイトル画面のクラス。
/// </summary>
class TitleScene : public YKEngine::BaseScene, private TitleSceneStateContext
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

	/// <summary>
	/// レベルの生成。
	/// </summary>
	void CreateLevel();

	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Audio* audio_;
	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;
	
	//オーディオマネージャー
	AudioManager* audioManager_;

	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	std::unique_ptr<CameraManager> cameraManager_;

	std::unique_ptr<TitleUI> titleUI_;

	SceneChangeStaging* sceneChangeStaging_ = nullptr;//シーンチェンジ演出

	//レールムーバー
	std::unique_ptr<RailMover> railMover_;

	//ステージオブジェクト
	std::unique_ptr<StageObjects> stageObjects_;

	//ステートマシン
	std::unique_ptr<YKEngine::StateMachine<TitleSceneStateContext>> stateMachine_;

};

