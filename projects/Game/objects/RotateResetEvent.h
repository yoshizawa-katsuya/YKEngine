#pragma once
#include "BaseEvent.h"

class RotateResetEvent : public BaseEvent
{
public:

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

};

