#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Animation.h"
#include "Input.h"
#include "Sprite.h"
#include <numbers>
#include "Collider.h"
#include "ContactRecord.h"
class GameScene;
class Camera;
class MapChipField;
class BaseEnemy;

class Player : public Collider
{
public:

	Player();
	~Player() override;

	void Initialize(BaseModel* model) override;

	void Update() override;

	void Draw(Camera* camera) override;

	void HUDInitialize(uint32_t textureHandleHeartFrame, uint32_t textureHandleHeart);

	void HUDDraw();

	void OnCollision() override;

	void HammerInitialize(BaseModel* model);

	//攻撃が敵に当たった時の処理
	void AttackHit(BaseEnemy* enemy);

	//void Draw(Camera* camera) override;

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool GetIsAttack() { return workAttack_.isAttack_; }

	LRDirection GetLRDirection() { return lrDirection_; }

	const Vector2& GetAttackRange() const { return workAttack_.attackRange_; }

private:

	void HammerUpdate();

	//移動入力
	void Move();

	//移動
	void MoveAppli();

	//地面に接触している場合の処理
	void GroundCollision();

	//攻撃入力
	void Attack();

	//射撃入力
	void Fire();

	//攻撃更新
	void AttackUpdate();

	//無敵時間更新
	void InvincibleTimeUpdate();

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	float bulletSpped_ = 0.25f;

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

	struct MoveRange {
		MoveRange();
		float min_;
		float max_;
	};

	MoveRange moveRange_;

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

	//攻撃用ワーク
	struct WorkAttack {
		WorkAttack();
		//攻撃中か否か
		bool isAttack_;
		//攻撃力
		uint32_t attackPower_;
		//攻撃範囲
		Vector2 attackRange_;
		// 攻撃用の媒介変数
		uint16_t attackParameter_;
		//攻撃の全体フレーム
		uint16_t attackTime_;
		//振り払いの終わる時間
		uint16_t swingEndTime_;
		//接触記録
		ContactRecord contactRecord_;
	};
	
	WorkAttack workAttack_;

	//体力や無敵時間など
	struct StatusWork {
		StatusWork();
		//最大体力
		uint32_t maxHP_;
		//体力
		uint32_t HP_;
		//生存しているか否か
		bool isAlive_;
		//残りの無敵時間
		uint32_t remainingInvincibleTime_;
		//攻撃を喰らった際の無敵時間
		uint32_t maxInvincibleTime_;
	};
	
	StatusWork status_;

	struct HUD {
		std::vector<std::unique_ptr<Sprite>> spriteHeartFrame_;
		std::vector<std::unique_ptr<Sprite>> spriteHeart_;
	};

	HUD hud_;

	struct Hammer {
		WorldTransform worldTransform_;
		std::unique_ptr<Base3dObject> object_;
	};

	Hammer hammer_;
};

