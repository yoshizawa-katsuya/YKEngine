#pragma once
#include "Vector3.h"
#include <vector>
#include <memory>
#include "InstancingObjects.h"
class GameScene; // 前方宣言

// 敵の出現データを保持する構造体
struct EnemySpawn
{
	uint32_t waveNumber;        // ウェーブ番号
    Vector3 position;       // 出現位置
	Vector3 rotation;		// 進行方向の回転角度（例：0, 0, 0）
	std::vector<Vector3> controlPoints;	// スプライン曲線制御点(通過点)
};

class EnemySpawnManager
{
public:

	void Initialize(GameScene* scene); // 初期化メソッド

	void Update(); // 更新メソッド

	void Draw(Camera* camera); // 描画メソッド

    void AddSpawnData(uint32_t waveNumber, const Vector3& position, const Vector3& rotation, const std::vector<Vector3>& controlPoints);

	void WaveStart(uint32_t waveNum); // ウェーブ開始メソッド

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

	GameScene* gameScene_; // ゲームシーンへのポインタ

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

