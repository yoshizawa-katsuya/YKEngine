#pragma once
#include "Collider.h"

class Enemy : public Collider
{
public:
	Enemy();
	~Enemy() override;

	void Initialize(BaseModel* model, const Vector3& translate = {0.0f, 0.5f, 0.0f});

	//void Update() override;

	//void Draw(Camera* camera) override;

private:

};

