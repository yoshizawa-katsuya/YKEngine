#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Animation.h"
#include "Input.h"
#include <numbers>
class Camera;
class MapChipField;

class Player
{
public:

	Player();

	void Initialize(BaseModel* model);

	void Update();

	void Draw(Camera* camera);

private:

	//移動入力
	void Move();

	//移動
	void MoveAppli();

	//地面に接触している場合の処理
	void GroundCollision();

	//Transform変数を作る
	WorldTransform worldTransform_;

	std::unique_ptr<Base3dObject> object_;

	Input* input_;

	enum  class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_;
	// 左右の自キャラ角度テーブル
	std::vector<float> destinationRotationYTable = { std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f };
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

	float kLimitFallSpeed_;
};

