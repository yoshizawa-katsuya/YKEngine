#include "EnemySpawnManager.h"
#include "ModelPlatform.h"

using namespace YKEngine;

void EnemySpawnManager::Initialize()
{
	uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/white.png");
	model_ = ModelPlatform::GetInstance()->CreateSphere(textureHandle, "EnemySpawn");

}

void EnemySpawnManager::Update()
{
	//完了した敵出現オブジェクトを削除
	std::erase_if(enemySpawnObjects_, [](const std::unique_ptr<EnemySpawnObject>& obj) { return obj->IsCompleted(); });

	for (std::unique_ptr<EnemySpawnObject>& spawnObject : enemySpawnObjects_)
	{
		spawnObject->Update();
	}
	
}

void EnemySpawnManager::Draw(Camera* camera)
{
	for (std::unique_ptr<EnemySpawnObject>& spawnObject : enemySpawnObjects_)
	{
		spawnObject->Draw(camera);
	}
}

void EnemySpawnManager::AddSpawnData(const EnemySpawn& spawnData)
{
	spawnDatas_.push_back(spawnData);
}

void EnemySpawnManager::WaveStart(uint32_t waveNum)
{
	waveNumber_ = waveNum;

	std::vector<EnemySpawn> nowWaveDatas;	//今回のウェーブで出現する敵データを格納する配列
	for (std::vector<EnemySpawn>::iterator spawnData = spawnDatas_.begin(); spawnData != spawnDatas_.end();)
	{
		if (spawnData->waveNumber <= waveNumber_)
		{
			nowWaveDatas.push_back(*spawnData);	//今回のウェーブで出現する敵データを追加
			spawnData = spawnDatas_.erase(spawnData);	//出現させる敵のデータを削除
		}
		else
		{
			++spawnData;	//次の敵出現データへ
		}
	}

	std::unique_ptr<EnemySpawnObject>& spawnObject = enemySpawnObjects_.emplace_back();
	spawnObject = std::make_unique<EnemySpawnObject>();
	spawnObject->Initialize(nowWaveDatas, model_);
	spawnObject->SetEnemyManager(enemyManager_);
}



