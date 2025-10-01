#pragma once
#include <memory>
#include "InstancingObjects.h"
#include "EnemySpawn.h"
class EnemyManager;

class EnemySpawnManager
{
public:

	void Initialize(); // 初期化メソッド

	void Update(); // 更新メソッド

	void Draw(Camera* camera); // 描画メソッド

    void AddSpawnData(const EnemySpawn& spawnData);

	void WaveStart(uint32_t waveNum); // ウェーブ開始メソッド

	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; } // 敵管理クラスのポインタを設定する

private:
	
	void UpdateWait(); // 待機更新メソッド

	void UpdateWaveStart(); // ウェーブ開始更新メソッド

	void UpdateWaveInterval(); // ウェーブ中間更新メソッド

	void UpdateWaveEnd(); // ウェーブ終了更新メソッド

	void SpawnEnemies(); // 敵出現メソッド

	//フェーズ
	enum class Phase {
		kWait,	//待機部
		kWaveStart,	//ウェーブ開始部
		kWaveInterval, //ウェーブ中間部
		kWaveEnd,	//ウェーブ終了部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kWait;

	EnemyManager* enemyManager_ = nullptr; // 敵管理クラスのポインタ

	std::vector<EnemySpawn> spawnDatas_; // 敵の出現データリスト

	uint32_t waveNumber_ = 0; // 現在のウェーブインデックス

	std::unique_ptr<InstancingObjects> objects_;
	
	const float deltaTime_ = 1.0f / 60.0f; // 仮のデルタタイム（60FPS想定）
	const float switchTime_ = 0.3f; // フェーズ切り替え時間
	const float intervalTime_ = 0.2f; // インターバル時間
	float timer_ = 0.0f; // タイマー
	std::vector<WorldTransform> worldTransforms_; // ワールド変換行列のリスト

	//一度に出現させる数
	uint32_t spawnCount_ = 0;
};

