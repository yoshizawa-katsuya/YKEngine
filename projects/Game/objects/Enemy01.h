#pragma once
#include "BaseEnemy.h"

class Enemy01 : public BaseEnemy
{
public:
	Enemy01();
	~Enemy01() override;

	void Initialize(BaseModel* model, const EulerTransform& transform) override;

	void Update() override;

	void SetHPGaugeModel(BaseModel* model, BaseModel* frameModel) override;

private:

	

};

