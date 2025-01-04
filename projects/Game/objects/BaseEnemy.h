#pragma once
#include "Collider.h"
#include <numbers>

class BaseEnemy : public Collider
{
public:
	BaseEnemy();
	~BaseEnemy() override;

	virtual void Initialize(BaseModel* model, const EulerTransform& transform);

	//void Update() override;

	//void Draw(Camera* camera) override;

	virtual uint32_t GetSerialNumber() const { return serialNumber_; }

protected:

	//シリアルナンバー
	uint32_t serialNumber_;

	//次のシリアルナンバー
	static uint32_t nextSerialNumber_;

	// 左右の角度テーブル
	std::vector<float> destinationRotationYTable = { std::numbers::pi_v<float> / 2.0f + 0.3f, std::numbers::pi_v<float> *3.0f / 2.0f - 0.3f };

	LRDirection lrDirection_;

};

