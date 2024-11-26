#pragma once
#include "Base3dObject.h"
#include "BaseModel.h"
#include "SkinModel.h"
#include "WorldTransform.h"
#include "Animation.h"

class Camera;
class PlayerLockOn;

class Boss
{

public:

	void Initialize(const std::vector<BaseModel*>& models);

	void Update();

	void Draw(Camera* camera);

	void SetLockOn(PlayerLockOn* lockOn) { lockOn_ = lockOn; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void OnCollision();

	// ボスのHPを取得
	float GetBossHP() { return bossHP; }

	float GetBossMaxHP() { return bossMaxHP; }
private:

	//Transform変数を作る
	WorldTransform worldTransform_;
	std::vector<WorldTransform> worldTransforms_;

	//オブジェクト
	std::vector<std::unique_ptr<Base3dObject>> objects_;
	PlayerLockOn* lockOn_ = nullptr;

	// ボスのHP上限
	const float bossMaxHP = 100.0f;

	// ボスの現在HP
	float bossHP;
};


