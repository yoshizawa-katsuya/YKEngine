#pragma once
#include "BaseEnemy.h"

class Enemy03 : public BaseEnemy
{
public:
	Enemy03();
	~Enemy03() override;

	void Initialize(BaseModel* model, const EulerTransform& transform) override;

	void SetHPGaugeModel(BaseModel* model) override;

private:

	void HPGaugeUpdate() override;

};