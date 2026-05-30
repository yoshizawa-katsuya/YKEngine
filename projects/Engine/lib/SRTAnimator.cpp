#include "SRTAnimator.h"
#include "Lerp.h"

using namespace YKEngine;

void YKEngine::SRTAnimator::SetAnimation(const Vector3& startVector, const Vector3& endVector, float duration, bool isPingPong)
{
	startVector_ = startVector;
	endVector_ = endVector;
	duration_ = duration;
	elapsedTime_ = 0.0f;
	isEnd_ = false;
	isPingPong_ = isPingPong;
}

Vector3 SRTAnimator::Update()
{
	//経過時間を更新する
	UpdateElapsedTime(elapsedTime_);
	
	return Lerp(startVector_, endVector_, ApplyEasing(easingType_, elapsedTime_ / duration_));
}

Vector3 SRTAnimator::Update(float& elapsedTime)
{
	//経過時間を更新する
	UpdateElapsedTime(elapsedTime);

	return Lerp(startVector_, endVector_, (elapsedTime / duration_));
}

void YKEngine::SRTAnimator::UpdateElapsedTime(float& elapsedTime)
{
	//経過時間が所要時間に達していたら、これ以上経過時間を更新しない
	if (elapsedTime >= duration_)
	{
		return;
	}

	elapsedTime += dxCommon_->GetDeltaTime();
	if (elapsedTime > duration_)
	{
		elapsedTime = duration_;
		isEnd_ = true;
	}
	//往復アニメーションするなら、アニメーションが終わったときに開始ベクトルと終了ベクトルを入れ替える
	if (isPingPong_ && isEnd_)
	{
		std::swap(startVector_, endVector_);
		elapsedTime_ = 0.0f;
		isEnd_ = false;
	}
}
