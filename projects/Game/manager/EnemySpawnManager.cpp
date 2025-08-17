#include "EnemySpawnManager.h"
#include "GameScene.h"

void EnemySpawnManager::Initialize(GameScene* scene)
{
	gameScene_ = scene;
}

void EnemySpawnManager::Update()
{
	if (!isWaveStart_) 
	{
		return;	//ウェーブ開始前は何もしない
	}

	isWaveStart_ = false;	//ウェーブ開始フラグをリセット
	for (std::vector<EnemySpawn>::iterator spawnData = spawnDatas_.begin(); spawnData != spawnDatas_.end();)
	{
		if (spawnData->waveNumber <= waveNumber_)
		{
			gameScene_->EnemyPop(spawnData->position, spawnData->rotation);
			spawnData = spawnDatas_.erase(spawnData);	//出現した敵のデータを削除
		}
		else
		{
			++spawnData;	//次の敵出現データへ
		}
	}
}

void EnemySpawnManager::AddSpawnData(uint32_t waveNumber, const Vector3& position, const Vector3& rotation)
{
	EnemySpawn data;
	data.waveNumber = waveNumber;
	data.position = position;
	data.rotation = rotation;
	spawnDatas_.push_back(data);
}

void EnemySpawnManager::WaveStart(uint32_t waveNum)
{
	isWaveStart_ = true;
	waveNumber_ = waveNum;
}
