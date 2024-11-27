#pragma once
#include "Base3dObject.h"
#include "BaseModel.h"
#include "SkinModel.h"
#include "WorldTransform.h"
#include "Animation.h"

#include "BossCanon.h"

class Camera;
class Player;
class PlayerLockOn;

class Boss
{

public:

	void Initialize(const std::vector<BaseModel*>& models, BaseModel* canonModel);

	void Update(Camera* camera);

	void Draw(Camera* camera);

	void Attack(Camera* camera);

	void SetLockOn(PlayerLockOn* lockOn) { lockOn_ = lockOn; }

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void OnCollision();

	uint32_t GetBossHP() { return bossHP; }

	const uint32_t GetBossMaxHP() { return bossMaxHP; }

	bool IsDead() { return isDead_; }
private:

	//Transform変数を作る
	std::vector<WorldTransform> worldTransforms_;

	// 砲撃
	std::list<std::unique_ptr<BossCanon>> canons_;
	float coolTime_ = 5.0f;

	Player* player_ = nullptr;

	//オブジェクト
	std::vector<std::unique_ptr<Base3dObject>> objects_;
	std::unique_ptr<Base3dObject> canonObject_;
	PlayerLockOn* lockOn_ = nullptr;


private:
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	// ボスのHP上限
	const uint32_t bossMaxHP = 100;

	// ボスの現在HP
	uint32_t bossHP;

	bool isDead_;

};


