#pragma once
#include <memory>
#include "InstancingObjects.h"
#include "EnemySpawn.h"
#include "EnemySpawnObject.h"
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

	EnemyManager* enemyManager_ = nullptr; // 敵管理クラスのポインタ

	std::shared_ptr<BaseModel> model_;

	std::vector<EnemySpawn> spawnDatas_; // 敵の出現データリスト
	std::vector<std::unique_ptr<EnemySpawnObject>> enemySpawnObjects_; // 敵出現オブジェクトリスト

	uint32_t waveNumber_ = 0; // 現在のウェーブインデックス

};

