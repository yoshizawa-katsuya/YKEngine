#pragma once
#include "SRTAnimator.h"
#include "Animation.h"
#include "Input.h"
#include "Sprite.h"
#include "BaseCharacter.h"
#include "ReticleController.h"
class Camera;
class BaseEnemy;
class PlayerBulletManager;

/// <summary>
/// プレイヤークラス。
/// プレイヤーの移動、攻撃、チャージ、UI描画などを行う。
/// BaseCharacterを継承。
/// 位置はRailMoverを親に持つWorldTransformで管理。
/// </summary>
class Player : public BaseCharacter
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="viewPortMatrix">ビューポート行列。レティクルをワールド座標に変換するために使用。</param>
	/// <param name="parent">親のワールド変換</param>
	/// <param name="heartTextureHandle">ハートテクスチャのハンドル</param>
	/// <param name="heartEmptyTexturehandle">空のハートテクスチャのハンドル</param>
	void Initialize(BaseModel* model, Matrix4x4* viewPortMatrix, WorldTransform* parent, uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle);

	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="railCamera">レールカメラ。レティクルをワールド座標に変換するために使用。</param>
	void Update(Camera* railCamera);

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突したコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// UI描画。
	/// </summary>
	void DrawUI();

	/// <summary>
	/// ロックオンターゲットの設定。
	/// </summary>
	/// <param name="enemies">敵のリスト</param>
	/// <param name="railCamera">レールカメラ。敵の位置をスクリーン座標に変換するために使用。</param>
	void SetLockOnTarget(const std::list<std::unique_ptr<BaseEnemy>>& enemies, Camera* railCamera);

	/// <summary>
	/// ワールド座標の取得。
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 自機の逆向きベクトルの取得。
	/// </summary>
	Vector3 GetInverseLocalDirection();

	/// <summary>
	/// ゲームオーバーになった瞬間の回転。
	/// </summary>
	void GameOverRotate();

	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	bool GetIsGameOverEnd() { return isGameOverEnd_; }

	void SetPlayerBulletManager(PlayerBulletManager* playerBulletManager) { playerBulletManager_ = playerBulletManager; }

	//スタート処理が終わっていたらtrue
	bool StartCompleted() { return phase_ != Phase::Start; }

	/// <summary>
	/// ゲームオーバーになったことを伝える。
	/// </summary>
	void SetGameOver() { phase_ = Phase::GameOver; }

private:

	/// <summary>
	/// HUDの初期化。
	/// </summary>
	/// <param name="heartTextureHandle">ハートテクスチャのハンドル</param>
	/// <param name="heartEmptyTexturehandle">空のハートテクスチャのハンドル</param>
	void HUDInitialize(uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle);

	/// <summary>
	/// 移動入力の処理
	/// </summary>
	void HandleMoveInput();

	/// <summary>
	/// 開始部の更新
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// メインの更新
	/// </summary>
	void UpdateMain(Camera* railCamera);

	/// <summary>
	/// ゲームオーバー時の更新
	/// </summary>
	void UpdateGameOver();

	/// <summary>
	/// 回転処理。レティクルの方向に向く。
	/// </summary>
	void Rotate();

	/// <summary>
	/// レティクルの更新。
	/// </summary>
	/// <param name="railCamera">レールカメラ。レティクルをワールド座標に変換するために使用。</param>
	void ReticleUpdate(Camera* railCamera);

	/// <summary>
	/// 攻撃処理。
	/// </summary>
	void Attack();

	/// <summary>
	/// チャージ処理。
	/// </summary>
	void Charge();

	/// <summary>
	/// チャージリセット処理。
	/// </summary>
	void ChargeReset();

	//キーボード入力
	Input* input_ = nullptr;

	enum class Phase 
	{
		Start,	// 開始
		Main,	// メイン
		GameOver, // ゲームオーバー
	};
	//フェーズ
	Phase phase_ = Phase::Start;

	//自機の弾のマネージャー
	PlayerBulletManager* playerBulletManager_ = nullptr;

	std::unique_ptr<ReticleController> reticleController_ = nullptr;

	std::unique_ptr<SRTAnimator> startAnime_;

	Vector3 direction_{};	//方向

	const int kMaxHitPoint_ = 5; // 最大ヒットポイント
	int hitPoint_ = kMaxHitPoint_; // プレイヤーのヒットポイント

	std::vector<std::unique_ptr<Sprite>> heratSprites_; // ヒットポイントのスプライト
	std::vector<std::unique_ptr<Sprite>> heratEmptySprites_; // ヒットポイントがないときのスプライト

	float chargeTime_ = 0.0f; // チャージ時間
	const float kMaxChargeTime_ = 1.0f; // 最大チャージ時間
	bool isChargeMax_ = false; // チャージが最大かどうか

	//ゲームオーバー時の時間を計る変数
	float gameOverTimer_ = 0.0f;
	bool isGameOverEnd_ = false;	//ゲームオーバー処理が終わっていたらtrue
};

