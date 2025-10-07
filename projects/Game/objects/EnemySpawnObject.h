#pragma once
#include <memory>
#include "InstancingObjects.h"
#include "EnemySpawn.h"
class EnemyManager;

class EnemySpawnObject
{
public:
	
	void Initialize(const std::vector<EnemySpawn>& nowSpanwDatas, std::shared_ptr<BaseModel> model);

	void Update();

	void Draw(Camera* camera);

	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; } // 敵管理クラスのポインタを設定する

	bool IsCompleted() const { return isCompleted_; } // 敵の出現が完了したかどうかを取得する

private:

	void UpdateWaveStart(); // ウェーブ開始更新メソッド

	void UpdateWaveInterval(); // ウェーブ中間更新メソッド

	void UpdateWaveEnd(); // ウェーブ終了更新メソッド

	void SpawnEnemies(); // 敵出現メソッド

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

