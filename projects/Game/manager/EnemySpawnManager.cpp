#include "EnemySpawnManager.h"
#include "GameScene.h"

void EnemySpawnManager::Initialize(GameScene* scene)
{
	gameScene_ = scene;
}

void EnemySpawnManager::Update(float currentTime)
{
	for (std::vector<EnemySpawn>::iterator spawnData = spawnDatas_.begin(); spawnData != spawnDatas_.end();)
	{
		if (spawnData->spawnTime <= currentTime)
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

void EnemySpawnManager::AddSpawnData(float spawnTime, const Vector3& position, const Vector3& rotation)
{
	EnemySpawn data;
	data.spawnTime = spawnTime;
	data.position = position;
	data.rotation = rotation;
	spawnDatas_.push_back(data);
}
