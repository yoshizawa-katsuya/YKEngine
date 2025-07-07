#pragma once
#include "BaseBullet.h"

//自キャラの弾
class PlayerBullet : public BaseBullet
{
public:

	// 初期化
	void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) override;

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

};