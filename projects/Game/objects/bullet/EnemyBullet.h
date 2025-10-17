#pragma once
#include "BaseBullet.h"
class Player;

/// <summary>
/// 敵弾クラス。
/// プレイヤーに当たるとダメージを与える。
/// </summary>
class EnemyBullet : public BaseBullet
{
public:

	//初期化
	void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle, Player* target, float speed);

	void Update(Camera* railCamera);

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

	void Move() override; 

	void Homig(); // ホーミング動作

	Player* target_ = nullptr;
	float speed_ = 0.5f;
	bool isHoming_ = true; // ホーミング弾かどうか
};