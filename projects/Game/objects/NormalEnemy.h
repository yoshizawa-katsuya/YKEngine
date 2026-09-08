#pragma once
#include "BaseEnemy.h"

/// <summary>
/// ノーマルエネミークラス
/// </summary>
class NormalEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// コンストラクタ。
	/// </summary>
	NormalEnemy() { hp_ = kMaxHp_; }

private:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="spawnData">敵の生成データ</param>
	/// <param name="target1">ターゲット1</param>
	/// <param name="target2">ターゲット2</param>
	void Initialize(const YKEngine::EnemySpawnData& spawnData, const BasePlayer* target1, const BasePlayer* target2) override;

	/// <summary>
	/// 移動。
	///	</summary>
	void Move() override;

	/// <summary>
	/// ターゲットを探索する。
	/// </summary>
	void SearchTarget();

	YKEngine::Vector3 RotateTowards(const YKEngine::Vector3& targetPosition, float deltaTime = 1.0f / 60.0f);

	float GetHomingPower(float distance);

private:

	const float kMaxHp_ = 8.0f;

    // 最大探索角度
	const float kMaxSearchAngle_ = 30.0f;

	// 最大ホーミング角度
	const float kMaxHomingAngle_ = 5.0f;

	// ターゲット1が角度内にいるかどうか
	bool isTarget1WithinAngle_ = false;
	// ターゲット2が角度内にいるかどうか
	bool isTarget2WithinAngle_ = false;

	// ホーミングターゲット
	const BasePlayer* homingTarget_ = nullptr;

	// ホーミング開始距離
	float kHomingStartDistance_ = 30.0f;
	// ホーミング終了距離
	float kHomingEndDistance_ = 5.0f;
};

