#pragma once
#include "BaseEnemy.h"

class TutorialEnemy01 : public BaseEnemy
{
public:
	TutorialEnemy01();

	~TutorialEnemy01() override;

	void Update() override;

	void SetHPGaugeModel(BaseModel* model, BaseModel* frameModel) override;

private:

};

