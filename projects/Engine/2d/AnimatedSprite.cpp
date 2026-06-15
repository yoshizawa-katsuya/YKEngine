#include "AnimatedSprite.h"

using namespace YKEngine;

void AnimatedSprite::Initialize(uint32_t textureHandle, int32_t horizontalDivisionNum, int32_t verticalDivisionNum)
{
	CommonInitialize(textureHandle, horizontalDivisionNum, verticalDivisionNum);
	horizontalIndex_ = 0;
	verticalIndex_ = 0;
	UpdateUV();

	isReverse_ = false;
	
}

void AnimatedSprite::Update()
{
	if (isLoop_) 
	{
		if (isReverse_)
		{
			ReverseLoopUpdate();
		}
		else
		{
			LoopUpdate();
		}
	}
	else
	{
		if (isReverse_)
		{
			ReverseNoLoopUpdate();
		}
		else
		{
			NoLoopUpdate();
		}
	}
}

void AnimatedSprite::InitializeReverseAnimation(uint32_t textureHandle, uint32_t horizontalDivisionNum, uint32_t verticalDivisionNum)
{
	CommonInitialize(textureHandle, horizontalDivisionNum, verticalDivisionNum);
	horizontalIndex_ = horizontalDivisionNum_ - 1;
	verticalIndex_ = verticalDivisionNum_ - 1;
	UpdateUV();

	isReverse_ = true;
}

void AnimatedSprite::Reset()
{
	horizontalIndex_ = 0;
	verticalIndex_ = 0;
	UpdateUV();

	isReverse_ = false;
	isEnd_ = false;
}

void AnimatedSprite::ResetReverseAnimation()
{
	horizontalIndex_ = horizontalDivisionNum_ - 1;
	verticalIndex_ = verticalDivisionNum_ - 1;
	UpdateUV();

	isReverse_ = true;
	isEnd_ = false;
}

void AnimatedSprite::CommonInitialize(uint32_t textureHandle, int32_t horizontalDivisionNum, int32_t verticalDivisionNum)
{
	Sprite::Initialize(textureHandle);

	horizontalDivisionNum_ = horizontalDivisionNum;
	verticalDivisionNum_ = verticalDivisionNum;
	//スプライトのサイズをアニメーション一枚分に変更
	size_.x /= static_cast<float>(horizontalDivisionNum_);
	size_.y /= static_cast<float>(verticalDivisionNum_);
	//UV移動量を計算
	horizontalMovingDistance_ = 1.0f / static_cast<float>(horizontalDivisionNum_);
	verticalMovingDistance_ = 1.0f / static_cast<float>(verticalDivisionNum_);
	//UVスケールをアニメーション一枚分に設定
	uvTransform_.scale.x = horizontalMovingDistance_;
	uvTransform_.scale.y = verticalMovingDistance_;

	isEnd_ = false;
}

void AnimatedSprite::LoopUpdate()
{
	//フレームカウントを進める
	currentFrame_++;
	//更新フレームに達していなかったら処理しない
	if (currentFrame_ < updateFrame_) 
	{
		return;
	}
	currentFrame_ = 0;

	//横方向インデックスが最大値に達したら折り返す
	if (horizontalIndex_ >= horizontalDivisionNum_)
	{
		horizontalIndex_ = 0;
		//縦方向インデックスを進める
		verticalIndex_++;

		// 縦方向インデックスも最大を超えたら折り返す
		if (verticalIndex_ >= verticalDivisionNum_)
		{
			verticalIndex_ = 0;
		}

	}

	UpdateUV();

	//横方向に進める
	horizontalIndex_++;
	
}

void AnimatedSprite::NoLoopUpdate()
{
	//アニメーション終了していたら処理しない
	if (isEnd_)
	{
		return;
	}
	//アニメーション終了判定
	if (horizontalIndex_ >= horizontalDivisionNum_ && verticalIndex_ >= verticalDivisionNum_ - 1)
	{
		isEnd_ = true;
		return;
	}

	LoopUpdate();

}

void AnimatedSprite::ReverseLoopUpdate()
{
	//フレームカウントを進める
	currentFrame_++;
	//更新フレームに達していなかったら処理しない
	if (currentFrame_ < updateFrame_)
	{
		return;
	}
	currentFrame_ = 0;

	//横方向インデックスが最低値に達したら折り返す
	if (horizontalIndex_ < 0)
	{
		horizontalIndex_ = horizontalDivisionNum_ - 1;
		//縦方向インデックスを進める
		verticalIndex_--;

		// 縦方向インデックスも最低を超えたら折り返す
		if (verticalIndex_ < 0)
		{
			verticalIndex_ = verticalDivisionNum_ - 1;
		}

	}

	UpdateUV();

	//横方向に進める
	horizontalIndex_--;
}

void AnimatedSprite::ReverseNoLoopUpdate()
{
	//アニメーション終了していたら処理しない
	if (isEnd_)
	{
		return;
	}
	//アニメーション終了判定
	if (horizontalIndex_ < 0 && verticalIndex_ <= 0)
	{
		isEnd_ = true;
		return;
	}

	ReverseLoopUpdate();

}

void AnimatedSprite::UpdateUV()
{
	uvTransform_.translation.x = horizontalMovingDistance_ * horizontalIndex_;
	uvTransform_.translation.y = verticalMovingDistance_ * verticalIndex_;
}
