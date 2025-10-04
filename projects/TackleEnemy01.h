#pragma once
#include "BaseEnemy.h"

class TackleEnemy01 : public BaseEnemy
{
public:

	~TackleEnemy01() override;

	void OnCollision([[maybe_unused]] Collider* other) override;

private:

	void UpdateApproach() override;
	void UpdateMain() override;
	void UpdateLeave() override;

	void Move() override;

	void SetColliderID() override;

	bool isHoming_ = true;

};

