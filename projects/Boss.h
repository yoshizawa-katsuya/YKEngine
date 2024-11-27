#pragma once
#include "Base3dObject.h"
#include "BaseModel.h"
#include "SkinModel.h"
#include "WorldTransform.h"
#include "Animation.h"

#include "BossBullet.h"
#include "BossCanon.h"

class Camera;
class Player;
class PlayerLockOn;

class Boss
{

public:

	void Initialize(const std::vector<BaseModel*>& models, BaseModel* canonModel,BaseModel* bulletModel);

	void Update(Camera* camera);

	void Draw(Camera* camera);

	void BulletAttack(Camera* camera);

	void CanonAttack(Camera* camera);
	
	void OnCollision();

	void SetLockOn(PlayerLockOn* lockOn) { lockOn_ = lockOn; }

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	uint32_t GetBossHP() { return bossHP; }

	const uint32_t GetBossMaxHP() { return bossMaxHP; }

	bool IsDead() { return isDead_; }

	const std::list<std::unique_ptr<BossCanon>>& GetCanons()const { return canons_; }
	const std::list<std::unique_ptr<BossBullet>>& GetBullets()const { return bullets_; }
private:

	//Transform変数を作る
	std::vector<WorldTransform> worldTransforms_;

	// 弾
	std::list<std::unique_ptr<BossBullet>> bullets_;
	// 発射間隔
	static const int kBulletAttackInterval = 20;
	// 発射タイマー
	int32_t bulletTimer_ = 0;

	// 砲撃
	std::list<std::unique_ptr<BossCanon>> canons_;
	// 発射間隔
	static const int kCanonAttackInterval = 60 * 8;
	// 発射タイマー
	int32_t canonTimer_ = 0;

	Player* player_ = nullptr;

	//オブジェクト
	std::vector<std::unique_ptr<Base3dObject>> objects_;
	std::unique_ptr<Base3dObject> canonObject_;
	std::unique_ptr<Base3dObject> bulletObject_;

	PlayerLockOn* lockOn_ = nullptr;

	// ボスのHP上限
	const uint32_t bossMaxHP = 100;

	// ボスの現在HP
	uint32_t bossHP;

	bool isDead_;


private:
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
};


