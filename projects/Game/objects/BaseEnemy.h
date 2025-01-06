#pragma once
#include "Collider.h"
#include <numbers>

class BaseEnemy : public Collider
{
public:
	BaseEnemy();
	virtual ~BaseEnemy() override;

	virtual void Initialize(BaseModel* model, const EulerTransform& transform);

	virtual void Update() override;

	void OnCollision() override;

	void Draw(Camera* camera) override;

	void HPGaugeDraw(Camera* camera, const Matrix4x4& billbordMatrix);

	void SetDarkRed(uint32_t textureHandle) { textureHandleDarkRed_ = textureHandle; }

	void SetHPGaugeModel(BaseModel* model);

	virtual void TakeHammer(uint32_t power);

	virtual uint32_t GetSerialNumber() const { return serialNumber_; }

	bool GetIsAlive() { return status_.isAlive_; }

protected:

	void HPGaugeUpdate();

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
		//爆弾の数
		uint32_t bombNum_;
		//生存しているか否か
		bool isAlive_;
	};

	StatusWork status_;

	//体力ゲージ
	struct HPGauge {
		WorldTransform worldTransform_;
		std::unique_ptr<Base3dObject> object_;
		WorldTransform frameWorldTransform_;
		std::unique_ptr<Base3dObject> frameObject_;
	};
	
	HPGauge hpGauge_;
	uint32_t textureHandleDarkRed_;
};

