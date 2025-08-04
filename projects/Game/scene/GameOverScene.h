#pragma once
#include "BaseScene.h"
#include "Fade.h"
class Input;
class ModelPlatform;

class GameOverScene : public BaseScene
{
public:

	~GameOverScene() override;

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

	//シーンのフェーズ
	enum class Phase {
		kStart,	//開始部
		kMain,	//メイン部
		kEnd,	//終了部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kStart;

	std::unique_ptr<Fade> fade_;

};

