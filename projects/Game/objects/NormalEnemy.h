#pragma once
#include "BaseEnemy.h"

/// <summary>
/// ノーマルエネミークラス
/// </summary>
class NormalEnemy : public BaseEnemy
{
protected:

	/// <summary>
	/// 移動。
	///	</summary>
	void Move() override;

private:

	void SearchTarget();

	YKEngine::Vector3 RotateTowards(const YKEngine::Vector3& targetPosition, float deltaTime = 1.0f / 60.0f);

	float GetHomingPower(float distance);

private:

    // 最大探索角度
	const float kMaxSearchAngle_ = 30.0f;

	const float kMaxHomingAngle_ = 5.0f;

	bool isTarget1WithinAngle_ = false;
	bool isTarget2WithinAngle_ = false;

	const BasePlayer* homingTarget_ = nullptr;

	float kHomingStartDistance_ = 30.0f;
	float kHomingEndDistance_ = 5.0f;

};

