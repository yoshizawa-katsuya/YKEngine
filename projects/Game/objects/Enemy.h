#pragma once
#include "BaseCharacter.h"
class Player;
class EnemyBulletManager;
class Camera;

//敵
class Enemy : public BaseCharacter
{
public:

	//デストラクタ
	~Enemy();

	// 初期化
	void Initialize(BaseModel* model, const Vector3& position, const Vector3& rotaion, Matrix4x4* viewPortMatrix, Camera* railCamera, const std::vector<Vector3>& controlPoints);

	
	// 更新
	void Update() override;

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//スクリーン座標を取得
	Vector2 GetScreenPosition(Camera* camera);

	// 弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	const float GetRadius() const { return radius_; }

	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }

private:
	
	void MainInitialize();

	void UpdateApproach();
	void UpdateMain();
	void UpdateLeave();

	//弾発射
	void Fire();

	//曲線の作成
	void CreateSplineCurve(const std::vector<Vector3>& controlPoints);

	//移動
	void Move();

	//回転
	void Rotate();

	//レールに沿って移動
	void MoveAlongRail();

	//自キャラ
	Player* player_ = nullptr;
	//敵の弾マネージャー
	EnemyBulletManager* enemyBulletManager_ = nullptr;

	// 弾
	//std::list<EnemyBullet*> enemyBullets_;
	//発射間隔
	static const int kFireInterval = 60;
	//発射タイマー
	int32_t fireTimer = 0;

	enum class Phase {
		Approach, // 接近する
		Main,	// メイン
		Leave,	// 離脱する
	};
	//フェーズ
	Phase phase_ = Phase::Approach;

	// キャラクターの移動速さ
	Vector3 velocity_ = {0.0f, 0.0f, 0.1f};

	//ビューポート行列
	Matrix4x4* viewPortMatrix_ = nullptr;

	int hitPoint_ = 3; // ヒットポイント

	Vector3 direction_; // 方向

	float leaveTimer_ = 0.0f; // 離脱タイマー

	Camera* railCamera_ = nullptr; // レールカメラ

	// スプライン曲線制御点(通過点)Add commentMore actions
	std::vector<Vector3> controlPoints_;
	// スプライン曲線上の点
	std::vector<Vector3> corvePoints_;
	// 線分の数
	const uint32_t segmentCount_ = 63;
	uint32_t moveCount_ = 0;

	float speed_ = 0.1f;

	bool hasRail_ = false; // レールがあるかどうか
};