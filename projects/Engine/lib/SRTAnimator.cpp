#include "SRTAnimator.h"
#include "Lerp.h"

using namespace YKEngine;

void SRTAnimator::SetAnimation(const Vector3& startVector, const Vector3& endVector, float duration)
{
	startVector_ = startVector;
	endVector_ = endVector;
	duration_ = duration;
	elapsedTime_ = 0.0f;
	isEnd_ = false;
}

Vector3 SRTAnimator::Update()
{
	if (elapsedTime_ < duration_) 
	{
		elapsedTime_ += dxCommon_->GetDeltaTime();
		if (elapsedTime_ > duration_) 
		{
			elapsedTime_ = duration_;
			isEnd_ = true;
		}
	}
	
	return Lerp(startVector_, endVector_, ApplyEasing(easingType_, elapsedTime_ / duration_));
}

Vector3 SRTAnimator::Update(float& elapsedTime)
{
	if (elapsedTime < duration_)
	{
		elapsedTime += dxCommon_->GetDeltaTime();
		if (elapsedTime > duration_)
		{
			elapsedTime = duration_;
			isEnd_ = true;
		}
	}

	return Lerp(startVector_, endVector_, (elapsedTime / duration_));
}
