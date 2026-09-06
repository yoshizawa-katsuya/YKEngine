#include "NormalEnemy.h"

#include "BasePlayer.h"

void NormalEnemy::Move()
{
	// ターゲットがまだ決まっていない場合は探索する
	if (!homingTarget_)
	{
		SearchTarget();
	}
	
	// ターゲットが決まっている、かつ角度内にいる場合はターゲット方向へ旋回する
	if (homingTarget_ && IsWithinAngle(homingTarget_->GetPosition(), kMaxHomingAngle_))
	{
		velocity_ = RotateTowards(homingTarget_->GetPosition());
	}

	velocity_ = YKEngine::Normalize(velocity_);
	velocity_ *= kMaxSpeed_;
}

void NormalEnemy::SearchTarget()
{
	isTarget1WithinAngle_ = IsWithinAngle(target1_->GetPosition(), kMaxSearchAngle_);
	isTarget2WithinAngle_ = IsWithinAngle(target2_->GetPosition(), kMaxSearchAngle_);

	if (isTarget1WithinAngle_ && isTarget2WithinAngle_)
	{
		float length1 = YKEngine::Length(worldTransform_.translation_ - target1_->GetPosition());
		float length2 = YKEngine::Length(worldTransform_.translation_ - target2_->GetPosition());

		if (length1 < length2)
		{
			homingTarget_ = target1_;
		}
		else
		{
			homingTarget_ = target2_;
		}
	}
	else if (isTarget1WithinAngle_)
	{
		homingTarget_ = target1_;
	}
	else if (isTarget2WithinAngle_)
	{
		homingTarget_ = target2_;
	}
	else
	{
		return;
	}
}

YKEngine::Vector3 NormalEnemy::RotateTowards(
	const YKEngine::Vector3& position,
	float deltaTime)
{
	YKEngine::Vector3 currentDirection = YKEngine::Normalize(velocity_);

	YKEngine::Vector3 targetDirection = YKEngine::Normalize(position - worldTransform_.translation_);

	float dot = YKEngine::Dot(currentDirection, targetDirection);
	dot = std::clamp(dot, -1.0f, 1.0f);

	float angle = acosf(dot);

	// ターゲットまでの距離
	float distance =
		YKEngine::Length(position - worldTransform_.translation_);

	// 距離からホーミング強度を計算
	float homingPower =
		(distance - kHomingEndDistance_) /
		(kHomingStartDistance_ - kHomingEndDistance_);

	homingPower = std::clamp(homingPower, 0.0f, 1.0f);

	// 距離に応じて最大旋回角度を変える
	float maxHomingAngle =
		kMaxHomingAngle_ * homingPower;

	// すでにほぼ同じ方向
	if (angle <= 0.0001f)
	{
		return targetDirection;
	}

	// 1フレームで到達できるなら、そのままターゲット方向へ
	if (angle <= maxHomingAngle)
	{
		return targetDirection;
	}

	// 回転軸
	YKEngine::Vector3 axis =
		YKEngine::Cross(currentDirection, targetDirection);

	axis = YKEngine::Normalize(axis);

	float cosAngle = cosf(maxHomingAngle);
	float sinAngle = sinf(maxHomingAngle);

	return currentDirection * cosAngle
		+ YKEngine::Cross(axis, currentDirection) * sinAngle;
}
float NormalEnemy::GetHomingPower(float distance)
{
	float t = (distance - kHomingEndDistance_) / (kHomingStartDistance_ - kHomingEndDistance_);

	return std::clamp(t, 0.0f, 1.0f);
}
