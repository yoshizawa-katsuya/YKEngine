#pragma once
#include "BaseCharacter.h"
class Player;
class GameScene;

//敵
class Enemy : public BaseCharacter
{
public:

	//デストラクタ
	~Enemy();

	// 初期化
	void Initialize(BaseModel* model, const Vector3& position);

	void ApproachInitialize();

	// 更新
	void Update() override;

	void ApproachUpdate();
	void LeaveUpdate();

	//弾発射
	void Fire();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//スクリーン座標を取得
	Vector3 GetScreenPosition(Camera* camera);

	// 弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	const float GetRadius() const { return radius_; }

	bool IsDead() const { return isDead_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	
	//自キャラ
	Player* player_ = nullptr;
	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 弾
	//std::list<EnemyBullet*> enemyBullets_;
	//発射間隔
	static const int kFireInterval = 60;
	//発射タイマー
	int32_t fireTimer = 0;

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	//フェーズ
	Phase phase_ = Phase::Approach;

	// キャラクターの移動速さ
	Vector3 ApproachVelocity_ = {0.0f, 0.0f, -0.1f};
	Vector3 LeaveVelocity_ = {0.1f, -0.1f, 0.0f};

};