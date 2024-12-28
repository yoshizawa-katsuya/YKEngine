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

	uint32_t GetSerialNumber() const { return serialNumber_; }

private:

	//シリアルナンバー
	uint32_t serialNumber_;

	//次のシリアルナンバー
	static uint32_t nextSerialNumber_;
};

