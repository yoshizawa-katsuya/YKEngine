#pragma once
#include "Vector3.h"
#include <vector>
class GameScene; // 前方宣言

// 敵の出現データを保持する構造体
struct EnemySpawn
{
    float spawnTime;        // 出現予定時刻（例：5.0秒後）
    Vector3 position;       // 出現位置
};

class EnemySpawnManager
{
public:

	void Initialize(GameScene* scene); // 初期化メソッド

	void Update(float currentTime); // 更新メソッド

    void AddSpawnData(float spawnTime, const Vector3& position);

private:
	
	GameScene* gameScene_; // ゲームシーンへのポインタ

	std::vector<EnemySpawn> spawnDatas_; // 敵の出現データリスト

};

