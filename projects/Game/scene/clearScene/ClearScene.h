#pragma once
#include "BaseScene.h"
#include "AnimatedSprite.h"
#include "My3dObject.h"
#include "RailMover.h"
#include "InstancingObjects.h"
#include "DirectionalLight.h"
#include "DemoPlayer.h"
#include "manager/CameraManager.h"
#include "StageObjects.h"
#include "StateMachine.hpp"
#include "ClearSceneStateContext.h"
class SceneChangeStaging;

/// <summary>
/// クリア画面を管理するクラス。
/// シーンの開始、更新、描画、終了を行う。
/// </summary>
class ClearScene : public YKEngine::BaseScene, private ClearSceneStateContext
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~ClearScene() override;

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
	/// レベルの生成。
	/// </summary>
	void CreateLevel();

	/// <summary>
	/// シーン遷移演出が終わっていたらtrueを返す。
	/// </summary>
	bool IsEndSceneChangeStaging() const override;

	/// <summary>
	/// 終了部が開始する際に行う処理。
	/// </summary>
	void EnterEnd() override;

	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	std::unique_ptr<CameraManager> cameraManager_;

	std::unique_ptr<YKEngine::Sprite> spriteBackGround_;

	SceneChangeStaging* sceneChangeStaging_ = nullptr;//シーンチェンジ演出

	//ステージオブジェクト
	std::unique_ptr<StageObjects> stageObjects_;

	//レールムーバー
	std::unique_ptr<RailMover> railMover_;

	//デモプレイヤー
	std::unique_ptr<DemoPlayer> demoPlayer_;

	//ステートマシン
	std::unique_ptr<YKEngine::StateMachine<ClearSceneStateContext>> stateMachine_;

};

