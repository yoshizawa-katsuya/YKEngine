#include "EnemySpawnManager.h"
#include "GameScene.h"
#include "Easing.h"
#include "Lerp.h"

void EnemySpawnManager::Initialize()
{
	uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/white.png");
	std::shared_ptr<BaseModel> model = ModelPlatform::GetInstance()->CreateSphere(textureHandle, "EnemySpawn");

	objects_ = std::make_unique<InstancingObjects>();
	objects_->Initialize(model.get(), 127);

	worldTransforms_.resize(30);
}

void EnemySpawnManager::Update()
{
	switch (phase_)
	{
	case Phase::kWait:
		UpdateWait();
		break;
	case Phase::kWaveStart:
		UpdateWaveStart();
		break;
	case Phase::kWaveInterval:
		UpdateWaveInterval();
		break;
	case Phase::kWaveEnd:
		UpdateWaveEnd();
		break;
	default:
		break;
	}
}

void EnemySpawnManager::Draw(Camera* camera)
{
	objects_->CameraUpdate(camera);
	objects_->Draw();
}

void EnemySpawnManager::AddSpawnData(uint32_t waveNumber, const Vector3& position, const Vector3& rotation, const std::vector<Vector3>& controlPoints)
{
	EnemySpawn data;
	data.waveNumber = waveNumber;
	data.position = position;
	data.rotation = rotation;
	data.controlPoints = controlPoints;
	spawnDatas_.push_back(data);
}

// TODO : Phase::kWait以外だと想定外の動作をするのを修正する
void EnemySpawnManager::WaveStart(uint32_t waveNum)
{
	phase_ = Phase::kWaveStart;
	waveNumber_ = waveNum;

	spawnCount_ = 0;

	for (std::vector<EnemySpawn>::iterator spawnData = spawnDatas_.begin(); spawnData != spawnDatas_.end();)
	{
		if (spawnData->waveNumber <= waveNumber_)
		{
			//サイズを超えたら拡張
			if (spawnCount_ >= worldTransforms_.size())
			{
				worldTransforms_.resize(worldTransforms_.size() + 10);
			}
			WorldTransform& worldTransform = worldTransforms_[spawnCount_];
			worldTransform.Initialize();
			worldTransform.translation_ = spawnData->position;
			worldTransform.rotation_ = spawnData->rotation;
			
			spawnCount_++;
		}
		++spawnData;	//次の敵出現データへ
	}
}

void EnemySpawnManager::UpdateWait()
{
}

void EnemySpawnManager::UpdateWaveStart()
{
	objects_->PreUpdate();

	timer_ += deltaTime_;
	// 経過時間をスイッチ時間で割る
	float time = timer_ / switchTime_;
	
	// イージングで変化させる
	Vector3 scale = Lerp(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), EaseInCubic(time));

	for (uint32_t i = 0; i < spawnCount_; i++)
	{
		WorldTransform& worldTransform = worldTransforms_[i];
		// スケールをイージングで変化させる
		worldTransform.scale_ = scale;
		worldTransform.UpdateMatrix();

		// ワールドトランスフォームをインスタンシングオブジェクトに追加
		objects_->WorldTransformUpdate(worldTransform);
	}

	if (timer_ >= switchTime_)
	{
		timer_ = 0.0f;
		phase_ = Phase::kWaveInterval;
	}
}

void EnemySpawnManager::UpdateWaveInterval()
{
	timer_ += deltaTime_;
	if (timer_ >= intervalTime_)
	{
		timer_ = switchTime_;
		phase_ = Phase::kWaveEnd;
		SpawnEnemies();

	}
}

void EnemySpawnManager::UpdateWaveEnd()
{
	objects_->PreUpdate();

	timer_ -= deltaTime_;
	// 経過時間をスイッチ時間で割る
	float time = timer_ / switchTime_;

	if (timer_ <= 0.0f)
	{
		timer_ = 0.0f;
		phase_ = Phase::kWait;
		spawnCount_ = 0;
	}

	// イージングで変化させる
	Vector3 scale = Lerp(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), EaseInCubic(time));

	for (uint32_t i = 0; i < spawnCount_; i++)
	{
		WorldTransform& worldTransform = worldTransforms_[i];
		// スケールをイージングで変化させる
		worldTransform.scale_ = scale;
		worldTransform.UpdateMatrix();

		// ワールドトランスフォームをインスタンシングオブジェクトに追加
		objects_->WorldTransformUpdate(worldTransform);
	}

	
}

void EnemySpawnManager::SpawnEnemies()
{
	for (std::vector<EnemySpawn>::iterator spawnData = spawnDatas_.begin(); spawnData != spawnDatas_.end();)
	{
		if (spawnData->waveNumber <= waveNumber_)
		{
			enemyManager_->PopEnemy(spawnData->position, spawnData->rotation, spawnData->controlPoints);
			spawnData = spawnDatas_.erase(spawnData);	//出現した敵のデータを削除
		}
		else
		{
			++spawnData;	//次の敵出現データへ
		}
	}
}


