#pragma once
#include <memory>
#include "InstancingObjects.h"
#include "EnemySpawn.h"
class EnemyManager;

/// <summary>
/// 敵出現オブジェクト。
/// 出現時のエフェクトも管理する。
/// 敵の出現そのものはEnemyManagerに任せる。
/// </summary>
class EnemySpawnObject
{
public:
	
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="nowSpanwDatas">今回の敵出現データリスト</param>
	/// <param name="model">敵出現エフェクトモデル</param>
	void Initialize(const std::vector<EnemySpawn>& nowSpanwDatas, std::shared_ptr<BaseModel> model);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera* camera);

	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; } // 敵管理クラスのポインタを設定する

	bool IsCompleted() const { return isCompleted_; } // 敵の出現が完了したかどうかを取得する

private:

	/// <summary>
	/// ウェーブ開始更新処理。
	/// </summary>
	void UpdateWaveStart();

	/// <summary>
	/// ウェーブ中間更新処理。
	/// </summary>
	void UpdateWaveInterval();

	/// <summary>
	/// ウェーブ終了更新処理。
	/// </summary>
	void UpdateWaveEnd();

	/// <summary>
	/// 敵出現処理。
	/// </summary>
	void SpawnEnemies();

	//フェーズ
	enum class Phase {
		kWaveStart,	//ウェーブ開始部
		kWaveInterval, //ウェーブ中間部
		kWaveEnd,	//ウェーブ終了部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kWaveStart;

	EnemyManager* enemyManager_ = nullptr; // 敵管理クラスのポインタ

	std::unique_ptr<InstancingObjects> objects_;

	std::vector<EnemySpawn> spawnDatas_; // 敵の出現データリスト

	const float deltaTime_ = 1.0f / 60.0f; // 仮のデルタタイム（60FPS想定）
	const float switchTime_ = 0.3f; // フェーズ切り替え時間
	const float intervalTime_ = 0.2f; // インターバル時間
	float timer_ = 0.0f; // タイマー
	std::vector<WorldTransform> worldTransforms_; // ワールド変換行列のリスト

	bool isCompleted_ = false; // 敵の出現が完了したかどうか

};

