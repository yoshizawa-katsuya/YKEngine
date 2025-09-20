#pragma once
#include "BaseEventTrigger.h"

class RotateEventTrigger : public BaseEventTrigger
{
public:

	void Initialize(uint32_t waveNumber, const Vector3& position, const Vector3& rotate, float radius);

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	const Vector3& GetRotate() const { return worldTransform_.rotation_; }

private:

};