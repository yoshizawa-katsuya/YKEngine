#include "AnimatedSprite.h"

void AnimatedSprite::Initialize(uint32_t textureHandle, uint32_t divisionNum)
{
	Sprite::Initialize(textureHandle);

	divisionNum_ = divisionNum;
	size_.x /= static_cast<float>(divisionNum_);
	textureSize_.x = size_.x;
}

void AnimatedSprite::Update()
{
	currentFrame_++;
	if (currentFrame_ < updateFrame_) {
		return;
	}
	currentFrame_ = 0;

	uvTransform_.translation.x += 1.0f / static_cast<float>(divisionNum_);
	if (uvTransform_.translation.x >= 1.0f) {
		uvTransform_.translation.x = 0.0f;
	}
}
