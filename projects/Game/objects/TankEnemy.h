#pragma once
#include "BaseEnemy.h"
class TankEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// コンストラクタ。
	/// </summary>
	TankEnemy() { hp_ = kMaxHp_; }

private:

	void Move() override;

private:

	const float kMaxHp_ = 10.0f;

};

