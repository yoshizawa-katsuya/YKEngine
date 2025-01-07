#pragma once
#include "BaseEnemy.h"

class Enemy02 : public BaseEnemy
{
public:
	Enemy02();
	~Enemy02() override;

	void Initialize(BaseModel* model, const EulerTransform& transform) override;

	void Update() override;

private:


	uint32_t interval_ = 60;
	float bulletSpeed_ = 0.2f;
};

