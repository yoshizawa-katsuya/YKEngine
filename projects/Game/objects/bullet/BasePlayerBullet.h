#pragma once
#include "BaseBullet.h"

/// <summary>
/// プレイヤー弾の基底クラス。
/// プレイヤー弾の種類ごとにクラスを作成し、BasePlayerBulletを継承して実装する。
/// </summary>
class BasePlayerBullet : public BaseBullet
{
public:

	// 初期化
	virtual void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) override;

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

};