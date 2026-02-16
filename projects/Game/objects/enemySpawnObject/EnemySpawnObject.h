#pragma once
#include <memory>
#include "InstancingObjects.h"
#include "EnemySpawn.h"
#include "StateMachine.hpp"
#include "EnemySpawnObjectStateContext.h"
class EnemyManager;

/// <summary>
/// 敵出現オブジェクト。
/// 出現時のエフェクトも管理する。
/// 敵の出現そのものはEnemyManagerに任せる。
/// </summary>
class EnemySpawnObject : private EnemySpawnObjectStateContext
{
public:
	
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="nowSpanwDatas">今回の敵出現データリスト</param>
	/// <param name="model">敵出現エフェクトモデル</param>
	void Initialize(const std::vector<EnemySpawn>& nowSpanwDatas, std::shared_ptr<YKEngine::BaseModel> model);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera"></param>
	void Draw(YKEngine::Camera* camera);

	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; } // 敵管理クラスのポインタを設定する

	bool IsCompleted() const { return isCompleted_; } // 敵の出現が完了したかどうかを取得する

private:

	/// <summary>
	/// ウェーブ開始更新処理。
	/// </summary>
	void UpdateWaveStart() override;

	/// <summary>
	/// ウェーブ中間更新処理。
	/// </summary>
	void UpdateWaveInterval() override;

	/// <summary>
	/// ウェーブ終了更新処理。
	/// </summary>
	void UpdateWaveEnd() override;

	/// <summary>
	/// 開始処理が完了していればtrueを返す。
	/// </summary>
	bool IsWaveStartEnd() const override { return timer_ >= kPhaseSwitchTime_; }

	/// <summary>
	/// 中間処理が完了していればtrueを返す。
	/// </summary>
	bool IsWaveIntervalEnd() const override { return timer_ >= kIntervalTime_; }

	/// <summary>
	/// タイマーをリセットする。
	/// </summary>
	void ResetTimer() override { timer_ = 0.0f; }

	/// <summary>
	/// タイマーを切り替え時間に初期化する。
	/// </summary>
	void SetTimerToSwitchTime() override { timer_ = kPhaseSwitchTime_; }

	/// <summary>
	/// 敵出現処理。
	/// </summary>
	void SpawnEnemies() override;

	// ステートマシン
	std::unique_ptr<YKEngine::StateMachine<EnemySpawnObjectStateContext>> stateMachine_;

	EnemyManager* enemyManager_ = nullptr; // 敵管理クラスのポインタ

	std::unique_ptr<YKEngine::InstancingObjects> objects_;

	std::vector<EnemySpawn> spawnDatas_; // 敵の出現データリスト

	const float kDeltaTime_ = 1.0f / 60.0f; // 仮のデルタタイム（60FPS想定）
	const float kPhaseSwitchTime_ = 0.3f; // フェーズ切り替え時間
	const float kIntervalTime_ = 0.2f; // インターバル時間
	float timer_ = 0.0f; // タイマー
	std::vector<YKEngine::WorldTransform> worldTransforms_; // ワールド変換行列のリスト

	bool isCompleted_ = false; // 敵の出現が完了したかどうか

};

