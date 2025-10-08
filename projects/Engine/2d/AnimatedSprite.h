#pragma once
#include "Sprite.h"

//アニメーション付きスプライト
class AnimatedSprite : public Sprite
{
public:

	//初期化
	void Initialize(uint32_t textureHandle, uint32_t divisionNum);

	//更新
	void Update();
	
	void SetUpdateFrame(uint32_t updateFrame) { updateFrame_ = updateFrame; }

private:

	//分割数
	uint32_t divisionNum_ = 4;

	//アニメーションの更新に必要なフレーム数
	uint32_t updateFrame_ = 1;

	//現在のフレーム数
	uint32_t currentFrame_ = 0;

};

