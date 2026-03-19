#pragma once
#include <memory>
#include "InstancingObjects.h"
#include "EnemySpawn.h"
#include "EnemySpawnObject.h"
#include "LevelDataLoader.h"
class EnemyManager;

/// <summary>
/// 敵の出現を管理するクラス。
/// 敵の生成そのものは、EnemySpawnObjectクラスで行う。
/// </summary>
class EnemySpawnManager
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">描画に使用するカメラ</param>
	void Draw(YKEngine::Camera* camera);

	/// <summary>
	/// 敵の出現データを取得する。
	/// </summary>
	/// <param name="outSpawnDatas">取得した敵の出現データを格納する配列</param>
	void GetSpawnDatas(const std::vector<YKEngine::EnemySpawnData>& outSpawnDatas);

	/// <summary>
	/// 指定したウェーブの敵出現を開始する。
	/// </summary>
	/// <param name="waveNum">開始するウェーブのインデックス</param>
	void WaveStart(uint32_t waveNum);

	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; } // 敵管理クラスのポインタを設定する

private:

	EnemyManager* enemyManager_ = nullptr; // 敵管理クラスのポインタ

	std::shared_ptr<YKEngine::BaseModel> model_;

	std::vector<EnemySpawn> spawnDatas_; // 敵の出現データリスト
	std::vector<std::unique_ptr<EnemySpawnObject>> enemySpawnObjects_; // 敵出現オブジェクトリスト

	uint32_t waveNumber_ = 0; // 現在のウェーブインデックス

	const std::unordered_map<std::string, EnemyType>& GetEnemyTypeMap() const; // 敵の種類と敵の種類の対応表を取得する

};

