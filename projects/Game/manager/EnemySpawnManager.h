#pragma once
#include "Vector3.h"
#include <vector>
class GameScene; // 前方宣言

// 敵の出現データを保持する構造体
struct EnemySpawn
{
	uint32_t waveNumber;        // ウェーブ番号
    Vector3 position;       // 出現位置
	Vector3 rotation;		// 進行方向の回転角度（例：0, 0, 0）
};

class EnemySpawnManager
{
public:

	void Initialize(GameScene* scene); // 初期化メソッド

	void Update(); // 更新メソッド

    void AddSpawnData(uint32_t waveNumber, const Vector3& position, const Vector3& rotation);

	void WaveStart(uint32_t waveNum); // ウェーブ開始メソッド

private:
	
	GameScene* gameScene_; // ゲームシーンへのポインタ

	std::vector<EnemySpawn> spawnDatas_; // 敵の出現データリスト

	uint32_t waveNumber_ = 0; // 現在のウェーブインデックス
	
	//ウェーブが始まった瞬間かどうか
	bool isWaveStart_ = false;
};

