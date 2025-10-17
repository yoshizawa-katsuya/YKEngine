#pragma once
#include "BaseScene.h"
#include "AnimatedSprite.h"
class Input;
class ModelPlatform;

/// <summary>
/// クリア画面を管理するクラス。
/// シーンの開始、更新、描画、終了を行う。
/// </summary>
class ClearScene : public BaseScene
{
public:

	~ClearScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	void UpdateStart();

	void UpdateMain();

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

