#pragma once
#include "BaseScene.h"
#include "AnimatedSprite.h"
class SceneChangeStaging;

namespace YKEngine
{
	class Input;
	class ModelPlatform;
}

/// <summary>
/// ゲームオーバー画面を管理するクラス。
/// ゲームオーバー画面の初期化、更新、描画、終了処理を行う。
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
	/// 終了処理。
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

	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	std::unique_ptr<YKEngine::Sprite> spriteBackGround_;
	std::unique_ptr<YKEngine::Sprite> spriteGameOverKeyBoard_;

	SceneChangeStaging* sceneChangeStaging_ = nullptr;//シーンチェンジ演出

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

