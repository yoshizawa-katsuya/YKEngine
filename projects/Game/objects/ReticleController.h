#pragma once
#include "WorldTransform.h"
#include <memory>
#include "Sprite.h"
class Input;
class Camera;
class BaseEnemy;

class ReticleController
{
public:

	void Initialize(Matrix4x4* viewPortMatrix);

	void Update(Camera* railCamera);

	void Draw();

	void SetLockOnTarget(const std::list<std::unique_ptr<BaseEnemy>>& enemies, Camera* railCamera);

	//チャージマックス時の処理
	void ChargeMax();

	//チャージリセット時の処理
	void ChargeReset();

	bool IsLockOn() const { return isLockOn_; }

	const Vector3& GetTargetPosition() const { return target_; }

	Vector3 Get3DReticlePosition() { return worldTransform3DReticle_.GetWorldPosition(); }

private:

	void ChargeUpdate();

	void LockOn(const Vector2& position, const Vector3& targetPosition);

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> spriteLargeReticle_;
	std::unique_ptr<Sprite> spriteSmallReticle_;

	bool isLockOn_ = false;

	Vector3 target_;

	//ビューポート行列
	Matrix4x4* viewPortMatrix_ = nullptr;

	Input* input_ = nullptr;

	Vector4 defaultColor_ = { 1.0f, 1.0f, 0.1f, 1.0f };
	Vector4 chargeMaxColor_ = { 0.0f, 0.0f, 1.0f, 1.0f };
	bool isChargeMax_ = false;
	float chargeMaxTimer_ = 0.0f;
	//チャージマックス時に色に変化し終わるまでの時間
	const float chargeMaxChangeTime_ = 0.1f;
};

