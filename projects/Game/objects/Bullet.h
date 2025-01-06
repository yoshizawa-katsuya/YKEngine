#pragma once
#include "Collider.h"

class Bullet : public Collider
{
public:

	// 初期化
	void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity);

	// 更新
	virtual void Update() override;

	virtual void OnCollision() override;

	bool GetIsAlive() { return isAlive_; }

protected:

	//速度
	Vector3 velocity_;

	//デスフラグ
	bool isAlive_ = true;
};

