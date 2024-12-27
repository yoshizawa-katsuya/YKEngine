#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Animation.h"
#include "Input.h"
#include <numbers>
#include "Collider.h"
class Camera;
class MapChipField;

class Player : public Collider
{
public:

	enum  class LRDirection {
		kRight,
		kLeft,
	};

	Player();
	~Player() override;

	void Initialize(BaseModel* model) override;

	void Update() override;

	//void Draw(Camera* camera) override;

	bool GetIsAttack() { return isAttack_; }

	LRDirection GetLRDirection() { return lrDirection_; }

	const Vector2& GetAttackRange() const { return attackRange_; }

private:

	//移動入力
	void Move();

	//移動
	void MoveAppli();

	//地面に接触している場合の処理
	void GroundCollision();

	//攻撃入力
	void Attack();

	Input* input_;

	LRDirection lrDirection_;
	// 左右の自キャラ角度テーブル
	std::vector<float> destinationRotationYTable = { std::numbers::pi_v<float> / 2.0f + 0.3f, std::numbers::pi_v<float> * 3.0f / 2.0f - 0.3f};
	//角度補完レート
	float angleCompletionRate_;
	//目標角度
	float targetAngle_;
	//旋回開始時の角度
	float turnFirstRotationY_;
	//旋回タイマー
	float turnTimer_;

	//移動の速さ
	float speed_;

	//速度
	Vector3 velocity_;
	//接地状態か否か
	bool onGround_;
	//ジャンプ時の加速度
	float kJumpAcceleration_;
	//重力加速度
	float kGravityAcceleration_;
	//落下速度の限界
	float kLimitFallSpeed_;

	//攻撃中か否か
	bool isAttack_;
	//攻撃範囲
	Vector2 attackRange_;
};

