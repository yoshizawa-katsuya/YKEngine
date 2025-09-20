#pragma once
#include "BaseEventTrigger.h"

class RotateResetEventTrigger : public BaseEventTrigger
{
public:

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

};

