#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "BaseCharacter.h"
#include <optional>
class LockOn;
#include "Hammer.h"

///< summary>
/// 自キャラ
///  </summary>
class Player : public BaseCharacter{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models_">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) override;

	//調整項目の適用
	void ApplyGlobalVariables();

	//通常行動初期化
	void BehaviorRootInitialize();

	//攻撃行動初期化
	void BehaviorAttackInitialize();

	//ダッシュ行動初期化
	void BehaviorDashInitialize();

	//ジャンプ行動初期化
	void BehaviorJumpInitialize();

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();

	//腕振りギミック初期化
	void InitializeRollArmGimmick();

	/// <summary>
	/// 更新
	///  </summary>
	void Update() override;

	// 通常行動更新
	void BehaviorRootUpdate();

	//攻撃行動更新
	void BehaviorAttackUpdate();

	//ダッシュ行動初期化
	void BehaviorDashUpdate();

	//ジャンプ行動更新
	void BehaviorJumpUpdate();

	//浮遊ギミック更新
	void UpdateFloatingGimmick();

	//腕振りギミック更新
	void UpdateRollArmGimmick();

	/// <summary>
	/// 描画
	///  </summary>
	void Draw() override;

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	//衝突したとき
	void OnCollision([[maybe_unused]] Collider* other) override;

	//中心座標を取得
	Vector3 GetCenterPosition() const override;

	void SetCameraViewProjection(const ViewProjection* cameraViewProjection) { cameraViewProjection_ = cameraViewProjection; }

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	Hammer* GatHammer() { return hammer_.get(); }

private:

	//振るまい
	enum class Behavior {
		kRoot,	//通常状態
		kAttack,	//攻撃中
		kDash,	//ダッシュ中
		kJump,	//ジャンプ中
	};

	Behavior behavior_ = Behavior::kRoot;

	//次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	std::unique_ptr<Hammer> hammer_;

	const uint16_t kModelIndexBody = 0;
	const uint16_t kModelIndexHead = 1;
	const uint16_t kModelIndexL_arm = 2;
	const uint16_t kModelIndexR_arm = 3;
	const uint16_t kModelIndexHammer = 4;

	//カメラのビュープロジェクション
	const ViewProjection* cameraViewProjection_ = nullptr;

	//速度
	Vector3 velocity_ = {};

	//目標角度
	float targetAngle_;
	//角度補完レート
	const float angleCompletionRate_ = 0.2f;

	// 浮遊移動のサイクル<frame>
	int32_t floatingCycle_ = 90;

	// 浮遊の振幅<m>
	float floatingAmplitude_ = 0.3f;

	//浮遊ギミック用の媒介変数
	float floatingParameter_ = 0.0f;

	// 腕振りの振幅<m>
	float idleArmAngleMax_ = 0.5f;

	//腕振りギミック用の媒介変数
	float rollArmParameter_ = 0.0f;

	//攻撃用ワーク
	struct WorkAttack {
		// 攻撃用の媒介変数
		uint16_t attackParameter_ = 0;
	};

	WorkAttack workAttack_;

	
	

	// ダッシュ用ワーク
	struct  WorkDash {
		//ダッシュ用の媒介変数
		uint32_t dashParameter_ = 0;
	};

	WorkDash workDash_;
	
	// ロックオン
	const LockOn* lockOn_ = nullptr;

};