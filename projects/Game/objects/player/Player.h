#pragma once
#include "SRTAnimator.h"
#include "Animation.h"
#include "Input.h"
#include "Sprite.h"
#include "BaseCharacter.h"
#include "ReticleController.h"
#include "StateMachine.hpp"
#include "PlayerStateContext.h"
class BaseEnemy;
class PlayerBulletManager;

/// <summary>
/// プレイヤークラス。
/// プレイヤーの移動、攻撃、チャージ、UI描画などを行う。
/// BaseCharacterを継承。
/// 位置はRailMoverを親に持つWorldTransformで管理。
/// </summary>
class Player : public BaseCharacter, private PlayerStateContext
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="parent">親のワールド変換</param>
	void Initialize(YKEngine::WorldTransform* parent);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

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
	void SetLockOnTarget(const std::list<std::unique_ptr<BaseEnemy>>& enemies);

	/// <summary>
	/// ワールド座標の取得。
	/// </summary>
	/// <returns>ワールド座標</returns>
	YKEngine::Vector3 GetWorldPosition();

	/// <summary>
	/// 自機の逆向きベクトルの取得。
	/// </summary>
	YKEngine::Vector3 GetInverseLocalDirection();

	YKEngine::WorldTransform* GetWorldTransform() { return &worldTransform_; }

	bool GetIsGameOverEnd() { return isGameOverEnd_; }

	void SetPlayerBulletManager(PlayerBulletManager* playerBulletManager) { playerBulletManager_ = playerBulletManager; }

	void SetRailCamera(YKEngine::Camera* railCamera) { railCamera_ = railCamera; }

	//スタート処理が終わっていたらtrue
	bool StartCompleted() override { return startAnime_->GetIsEnd(); }

	/// <summary>
	/// ゲームクリアになったことを伝える。
	/// </summary>
	void SetGameClear() { isGameClear_ = true; }

	/// <summary>
	/// 死亡しているかどうか。
	/// </summary>
	/// <returns>死亡していたらtrue</returns>
	bool IsDead() override { return isDead_; }

	/// <summary>
	/// 死亡状態の設定。デバッグ用。
	/// </summary>
	/// <param name="isDead">死亡していたらtrue</param>
	void SetIsDead(bool isDead) { isDead_ = isDead; }

private:

	/// <summary>
	/// HUDの初期化。
	/// </summary>
	void HUDInitialize();

	/// <summary>
	/// 移動入力の処理
	/// </summary>
	void HandleMoveInput();

	/// <summary>
	/// 開始部の更新
	/// </summary>
	void UpdateStart() override;

	/// <summary>
	/// メインの更新
	/// </summary>
	void UpdateMain() override;

	/// <summary>
	/// ゲームオーバー時の更新
	/// </summary>
	void UpdateGameOver() override;

	/// <summary>
	/// ゲームクリア時の更新
	/// </summary>
	void UpdateGameClear() override;

	/// <summary>
	/// 開始処理が終わった後の処理。
	/// </summary>
	void AfterStartComplete() override;

	/// <summary>
	/// ゲームオーバーになった瞬間の回転。
	/// </summary>
	void GameOverRotate() override;

	/// <summary>
	/// 回転処理。レティクルの方向に向く。
	/// </summary>
	void Rotate();

	///<summary>
	/// 回転処理の共通部分。
	/// </summary>
	/// <returns>回転</returns>
	YKEngine::Vector3 RotateCommon();

	/// <summary>
	/// ハートの更新。
	/// </summary>
	void HeartUpdate();

	/// <summary>
	/// レティクルの更新。
	/// </summary>
	void ReticleUpdate();

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

	/// <summary>
	/// ダメージリアクションの初期化。
	/// </summary>
	void DamageReactionInitialize();

	/// <summary>
	/// ダメージリアクション
	/// </summary>
	void DamageReaction();

	bool GetIsGameClear() override { return isGameClear_; }

	//キーボード入力
	YKEngine::Input* input_ = nullptr;

	//ステートマシン
	std::unique_ptr<YKEngine::StateMachine<PlayerStateContext>> stateMachine_;

	//自機の弾のマネージャー
	PlayerBulletManager* playerBulletManager_ = nullptr;

	//レールカメラ
	YKEngine::Camera* railCamera_ = nullptr;

	//弾の発射間隔タイマー
	float shotIntervalTimer_ = 0.0f;

	std::unique_ptr<ReticleController> reticleController_ = nullptr;

	std::unique_ptr<YKEngine::SRTAnimator> startAnime_;
	std::unique_ptr<YKEngine::SRTAnimator> startRotateAnime_;

	YKEngine::Vector3 direction_{};	//方向

	const int kMaxHitPoint_ = 5; // 最大ヒットポイント
	int hitPoint_ = kMaxHitPoint_; // プレイヤーのヒットポイント

	std::vector<std::unique_ptr<YKEngine::Sprite>> heartSprites_; // ヒットポイントのスプライト
	std::vector<std::unique_ptr<YKEngine::Sprite>> heartEmptySprites_; // ヒットポイントがないときのスプライト

	float chargeTime_ = 0.0f; // チャージ時間
	const float kMaxChargeTime_ = 1.0f; // 最大チャージ時間
	bool isChargeMax_ = false; // チャージが最大かどうか

	//ゲームオーバー時の時間を計る変数
	float gameOverTimer_ = 0.0f;
	bool isGameOverEnd_ = false;	//ゲームオーバー処理が終わっていたらtrue

	//ダメージリアクションタイマー
	float damageReactionTimer_ = 0.0f;

	//ゲームクリアしていたらtrue
	bool isGameClear_ = false;
};

