#pragma once
#include "BaseEnemy.h"
class SwiftEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// コンストラクタ。
	/// </summary>
	SwiftEnemy() { hp_ = kMaxHp_; }

private:

	/// <summary>
	///  
	/// </summary>
	void Move() override;

	YKEngine::Vector3 CalculateCenterPosition();

	YKEngine::Vector3 RotateTowards(const YKEngine::Vector3& targetPosition, float deltaTime = 1.0f / 60.0f);

private:

	const float kMaxHp_ = 5.0f;

	// 最大ホーミング角度
	const float kMaxHomingAngle_ = 5.0f;

	// 目的地に到達したかどうか
	bool hasReachedDestination_ = false;
};

