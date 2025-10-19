#pragma once
#include "BaseScene.h"
#include "AnimatedSprite.h"
class Input;
class ModelPlatform;

/// <summary>
/// ゲームオーバー画面を管理するクラス。
/// ゲームオーバー画面の初期化、更新、描画、終了処理を行う。
/// </summary>
class GameOverScene : public BaseScene
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
	DirectXCommon* dxCommon_;

	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	std::unique_ptr<Sprite> spriteBackGround_;
	std::unique_ptr<AnimatedSprite> spriteSceneChange_;//シーンチェンジのスプライト

	//シーンのフェーズ
	enum class Phase {
		kStart,	//開始部
		kMain,	//メイン部
		kEnd,	//終了部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kStart;

};

