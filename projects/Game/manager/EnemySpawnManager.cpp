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

void EnemySpawnManager::GetSpawnDatas(const std::vector<EnemySpawnData>& outSpawnDatas)
{
	for (const EnemySpawnData& enemySpawnData : outSpawnDatas)
	{
		EnemySpawn enemySpawn;

		//敵の種類を取得
		if (enemySpawnData.type == "EnemySpawn")
		{
			enemySpawn.type = EnemyType::kShot01;
		}
		else if (enemySpawnData.type == "Enemy02Spawn")
		{
			enemySpawn.type = EnemyType::kShot02;
		}
		else if (enemySpawnData.type == "TackleEnemySpawn")
		{
			enemySpawn.type = EnemyType::kTackle01;
		}
		else if (enemySpawnData.type == "TackleEnemy02Spawn")
		{
			enemySpawn.type = EnemyType::kTackle02;
		}
		else
		{
			assert(0 && "不明な敵の種類です");
		}
		//レベルエディターで敵のwaveNumを必ず設定するようにする
		//敵のウェーブナンバーを取得
		enemySpawn.waveNumber = enemySpawnData.waveNum.value();

		//敵の発生位置を取得
		enemySpawn.position = enemySpawnData.transform.translation;
		//敵の回転を取得
		enemySpawn.rotation = enemySpawnData.transform.rotation;

		//敵の待機時間を取得
		if (enemySpawnData.waitTime.has_value())
		{
			enemySpawn.waitTime = enemySpawnData.waitTime.value();
		}

		//スプラインの制御点を取得
		if (enemySpawnData.spline.has_value())
		{
			enemySpawn.controlPoints = enemySpawnData.spline->controlPoints;
		}
		enemySpawn.speed = enemySpawnData.speed;

		//敵の出現データを追加
		spawnDatas_.push_back(enemySpawn);

	}
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



