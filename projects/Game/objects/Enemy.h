#pragma once
#include "BaseModel.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Rigid3dObject.h"
class Player;
class GameScene;

//敵
class Enemy {
public:

	//デストラクタ
	~Enemy();

	// 初期化
	void Initialize(BaseModel* model, uint32_t textureHandle, const Vector3& position);

	void ApproachInitialize();

	// 更新
	void Update();

	void ApproachUpdate();
	void LeaveUpdate();

	//弾発射
	void Fire();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 描画
	void Draw(Camera* camera);

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

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Rigid3dObject> object_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

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

	// 半径
	const float radius_ = 1.0f;

	//デスフラグ
	bool isDead_ = false;

};