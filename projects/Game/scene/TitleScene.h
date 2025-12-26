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
class SceneChangeStaging;

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

	/// <summary>
	/// 開始部の更新。
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// メイン部の更新。
	/// </summary>
	void UpdateMain();

	/// <summary>
	/// 終了部の更新。
	/// </summary>
	void UpdateEnd();

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
	
	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	std::unique_ptr<CameraManager> cameraManager_;

	std::unique_ptr<YKEngine::Sprite> spriteTitle_;//タイトルのスプライト

	SceneChangeStaging* sceneChangeStaging_ = nullptr;//シーンチェンジ演出

	std::shared_ptr<YKEngine::BaseModel> modelGround_;

	//テクスチャハンドル
	uint32_t textureHandle_;
	uint32_t textureHandleSkyBox_;

	//レールムーバー
	std::unique_ptr<RailMover> railMover_;

	//ステージオブジェクト
	std::unique_ptr<StageObjects> stageObjects_;

	//シーンのフェーズ
	enum class Phase
	{
		kStart,	//開始部
		kMain,	//メイン部
		kEnd,	//終了部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kStart;

};

