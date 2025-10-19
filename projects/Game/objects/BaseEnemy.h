#pragma once
#include "BaseCharacter.h"
#include "EnemySpawn.h"
class Player;
class EnemyBulletManager;
class Camera;

/// <summary>
/// 敵の基底クラス。
/// 継承して使用する。
/// </summary>
class BaseEnemy : public BaseCharacter
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	virtual ~BaseEnemy();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="spawnData">出現データ</param>
	/// <param name="viewPortMatrix">ビューポート行列</param>
	/// <param name="railCamera">レールカメラ。画面内に入っているか判定するために使用。</param>
	void Initialize(BaseModel* model, const EnemySpawn& spawnData, Matrix4x4* viewPortMatrix, Camera* railCamera);

	/// <summary>
	/// 更新。
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	virtual void OnCollision([[maybe_unused]] Collider* other) override;

	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ワールド座標を取得。
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// スクリーン座標を取得。
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns>スクリーン座標</returns>
	Vector2 GetScreenPosition(Camera* camera);

	// 弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	const float GetRadius() const { return radius_; }

	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }

protected:
	
	/// <summary>
	/// コライダーIDの設定。
	/// </summary>
	virtual void SetColliderID();

	/// <summary>
	/// メイン部の初期化。
	/// </summary>
	void MainInitialize();

	/// <summary>
	/// 接近更新。
	/// </summary>
	virtual void UpdateApproach();

	/// <summary>
	/// メイン更新。
	/// </summary>
	virtual void UpdateMain();

	/// <summary>
	/// 離脱更新。
	/// </summary>
	virtual void UpdateLeave();

	/// <summary>
	/// 弾の発射。
	/// </summary>
	void Fire();

	/// <summary>
	/// 移動に使うスプライン曲線の作成。
	/// </summary>
	/// <param name="controlPoints">制御点の配列</param>
	void CreateSplineCurve(const std::vector<Vector3>& controlPoints);

	/// <summary>
	/// 移動。
	/// </summary>
	virtual void Move();

	/// <summary>
	/// 回転。プレイヤーの方向を向く。
	/// </summary>
	void Rotate();

	/// <summary>
	/// レールに沿って移動。
	/// </summary>
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
	Vector3 velocity_ = {0.0f, 0.0f, 1.0f};

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