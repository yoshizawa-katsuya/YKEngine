#pragma once
#include "Collider.h"
#include <numbers>

class BaseEnemy : public Collider
{
public:
	BaseEnemy();
	virtual ~BaseEnemy() override;

	virtual void Initialize(BaseModel* model, const EulerTransform& transform);

	//void Update() override;

	//void Draw(Camera* camera) override;

	virtual void TakeHammer(uint32_t power);

	virtual uint32_t GetSerialNumber() const { return serialNumber_; }

	bool GetIsAlive() { return status_.isAlive_; }

protected:

	//シリアルナンバー
	uint32_t serialNumber_;

	//次のシリアルナンバー
	static uint32_t nextSerialNumber_;

	// 左右の角度テーブル
	std::vector<float> destinationRotationYTable = { std::numbers::pi_v<float> / 2.0f + 0.3f, std::numbers::pi_v<float> *3.0f / 2.0f - 0.3f };

	LRDirection lrDirection_;

	//体力や無敵時間など
	struct StatusWork {
		StatusWork();
		//最大体力
		uint32_t maxHP_;
		//体力
		int32_t HP_;
		//生存しているか否か
		bool isAlive_;
	};

	StatusWork status_;

};

