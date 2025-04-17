#include "SRTAnimator.h"
#include "Lerp.h"

void SRTAnimator::SetAnimation(const Vector3& startVector, const Vector3& endVector, float duration)
{
	startVector_ = startVector;
	endVector_ = endVector;
	duration_ = duration;
}

Vector3 SRTAnimator::Update()
{
	if (elapsedTime_ < duration_) 
	{
		elapsedTime_ += dxCommon_->GetDeltaTime();
		if (elapsedTime_ > duration_) 
		{
			elapsedTime_ = duration_;
		}
	}
	
	return Lerp(startVector_, endVector_, (elapsedTime_ / duration_));
}

Vector3 SRTAnimator::Update(float& elapsedTime)
{
	if (elapsedTime < duration_)
	{
		elapsedTime += dxCommon_->GetDeltaTime();
		if (elapsedTime > duration_)
		{
			elapsedTime = duration_;
		}
	}

	return Lerp(startVector_, endVector_, (elapsedTime / duration_));
}
