#pragma once
#include "Sprite.h"

//アニメーション付きスプライト
class AnimatedSprite : public Sprite
{
public:

	//初期化
	void Initialize(uint32_t textureHandle, int32_t horizontalDivisionNum, int32_t verticalDivisionNum);

	//更新
	void Update();

	//アニメーションを逆再生する際の初期化
	void InitializeReverseAnimation(uint32_t textureHandle, uint32_t horizontalDivisionNum, uint32_t verticalDivisionNum);

	//アニメーションをリセットする
	void Reset();

	//アニメーションをリセットして逆再生する
	void ResetReverseAnimation();

	void SetUpdateFrame(uint32_t updateFrame) { updateFrame_ = updateFrame; }

	void SetIsLoop(bool isLoop) { isLoop_ = isLoop; }

	void SetIsReverse(bool isReverse) { isReverse_ = isReverse; }

	bool GetIsEnd() { return isEnd_; }

private:

	//初期化の共通処理
	void CommonInitialize(uint32_t textureHandle, int32_t horizontalDivisionNum, int32_t verticalDivisionNum);

	//最後に到達したら最初に戻る更新
	void LoopUpdate();

	//最後に到達したら止まる更新
	void NoLoopUpdate();

	//アニメーションをループ逆再生する更新
	void ReverseLoopUpdate();

	//アニメーションを逆再生する更新
	void ReverseNoLoopUpdate();

	//更新の共通処理
	void CommonUpdate();

	//分割数
	int32_t horizontalDivisionNum_ = 1;
	int32_t verticalDivisionNum_ = 1;

	//現在のインデックス
	int32_t horizontalIndex_ = 0;
	int32_t verticalIndex_ = 0;

	//テクスチャの移動量
	float horizontalMovingDistance_ = 0.0f;
	float verticalMovingDistance_ = 0.0f;

	//アニメーションの更新に必要なフレーム数
	uint32_t updateFrame_ = 1;

	//現在のフレーム数
	uint32_t currentFrame_ = 0;

	//アニメーションが最後まで到達したか
	bool isEnd_ = false;

	//ループするかどうか
	bool isLoop_ = true;

	//逆再生するかどうか
	bool isReverse_ = false;
};

