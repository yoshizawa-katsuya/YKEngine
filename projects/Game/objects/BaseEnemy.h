#pragma once
#include "BaseCharacter.h"
#include "EnemySpawn.h"
class Player;
class EnemyBulletManager;
class YKEngine::Camera;

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
	void Initialize(YKEngine::BaseModel* model, const EnemySpawn& spawnData, YKEngine::Camera* railCamera);

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
	YKEngine::Vector3 GetWorldPosition();

	/// <summary>
	/// スクリーン座標を取得。
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns>スクリーン座標</returns>
	YKEngine::Vector2 GetScreenPosition(YKEngine::Camera* camera);

	const float GetRadius() const { return radius_; }

	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }

	bool IsDisappear() const { return isDisappear_; }

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
	/// 死亡更新。
	/// </summary>
	void UpdateDead();

	/// <summary>
	/// 弾の発射。
	/// </summary>
	void Fire();

	/// <summary>
	/// 移動に使うスプライン曲線の作成。
	/// </summary>
	/// <param name="controlPoints">制御点の配列</param>
	void CreateSplineCurve(const std::vector<YKEngine::Vector3>& controlPoints);

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

	/// <summary>
	/// ダメージリアクションの初期化。
	/// </summary>
	void DamageReactionInitialize();

	/// <summary>
	/// ダメージリアクション
	/// </summary>
	void DamageReaction();

	/// <summary>
	/// プレイヤーの弾と衝突したときの処理。
	/// </summary>
	/// <param name="bullet">衝突した弾</param>
	virtual void OnCollisionPlayerBullet(Collider* other);

	/// <summary>
	/// 死亡処理。
	/// </summary>
	/// <param name="bulletVelocity">弾の速度</param>
	/// <param name="bulletPosition">弾の位置</param>
	virtual void Die(const YKEngine::Vector3& bulletVelocity, const YKEngine::Vector3& bulletPosition);

	/// <summary>
	/// 消滅処理。
	/// </summary>
	void Disappear();

	//自キャラ
	Player* player_ = nullptr;
	//敵の弾マネージャー
	EnemyBulletManager* enemyBulletManager_ = nullptr;

	//発射間隔
	static const int kFireInterval = 60;
	//発射タイマー
	int32_t fireTimer = 0;

	//ダメージリアクションタイマー
	int32_t damageReactionTimer_ = 0;

	enum class Phase 
	{
		kApproach, // 接近する
		kMain,	// メイン
		kLeave,	// 離脱する
		kDead,	// 死亡
	};
	//フェーズ
	Phase phase_ = Phase::kApproach;

	// キャラクターの移動速さ
	YKEngine::Vector3 velocity_ = {0.0f, 0.0f, 1.0f};

	//ビューポート行列
	YKEngine::Matrix4x4* viewPortMatrix_ = nullptr;

	int hitPoint_ = 5; // ヒットポイント

	YKEngine::Vector3 direction_; // 方向

	float leaveTimer_ = 0.0f; // 離脱タイマー

	YKEngine::Camera* railCamera_ = nullptr; // レールカメラ

	// スプライン曲線制御点(通過点)Add commentMore actions
	std::vector<YKEngine::Vector3> controlPoints_;
	// スプライン曲線上の点
	std::vector<YKEngine::Vector3> corvePoints_;
	// 線分の数
	const uint32_t kSegmentCount_ = 63;
	uint32_t moveCount_ = 0;

	float speed_ = 0.1f;

	bool hasRail_ = false; // レールがあるかどうか

	//trueなら消滅処理を行う
	bool isDisappear_ = false;

	//死亡タイマー
	float deadTimer_ = 0.0f;

	//回転ベクトル
	YKEngine::Vector3 rotateVector_ = { 0.0f, 0.0f, 0.0f };

	//吹っ飛ぶ速さ
	float blowAwaySpeed_;
};