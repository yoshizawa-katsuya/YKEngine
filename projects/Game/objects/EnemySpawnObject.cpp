#include "EnemySpawnObject.h"
#include "Lerp.h"
#include "Easing.h"
#include "manager/EnemyManager.h"

void EnemySpawnObject::Initialize(const std::vector<EnemySpawn>& nowSpanwDatas, std::shared_ptr<BaseModel> model)
{
	phase_ = Phase::kWaveStart;

	spawnDatas_ = nowSpanwDatas;
	uint32_t enemyCount = static_cast<uint32_t>(spawnDatas_.size());
	worldTransforms_.resize(enemyCount);

	objects_ = std::make_unique<InstancingObjects>();
	objects_->Initialize(model.get(), enemyCount);

	//ワールド変換行列の初期化
	uint32_t i = 0;
	for (EnemySpawn& spawnData : spawnDatas_)
	{
		WorldTransform& worldTransform = worldTransforms_[i];
		worldTransform.Initialize();
		worldTransform.translation_ = spawnData.position;
		worldTransform.rotation_ = spawnData.rotation;
		i++;
	}

}

void EnemySpawnObject::Update()
{
	switch (phase_)
	{
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

void EnemySpawnObject::Draw(Camera* camera)
{
	objects_->CameraUpdate(camera);
	objects_->Draw();
}

void EnemySpawnObject::UpdateWaveStart()
{
	objects_->PreUpdate();

	timer_ += kDeltaTime_;
	// 経過時間をスイッチ時間で割る
	float time = timer_ / kPhaseSwitchTime_;

	// イージングで変化させる
	Vector3 scale = Lerp(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), EaseInCubic(time));

	for (WorldTransform& worldTransform : worldTransforms_)
	{
		// スケールをイージングで変化させる
		worldTransform.scale_ = scale;
		worldTransform.UpdateMatrix();
		// ワールドトランスフォームをインスタンシングオブジェクトに追加
		objects_->WorldTransformUpdate(worldTransform);
	}

	if (timer_ >= kPhaseSwitchTime_)
	{
		timer_ = 0.0f;
		phase_ = Phase::kWaveInterval;
	}
}

void EnemySpawnObject::UpdateWaveInterval()
{
	//時間経過で敵を出現させる
	timer_ += kDeltaTime_;
	if (timer_ >= kIntervalTime_)
	{
		timer_ = kPhaseSwitchTime_;
		phase_ = Phase::kWaveEnd;
		SpawnEnemies();

	}
}

void EnemySpawnObject::UpdateWaveEnd()
{
	objects_->PreUpdate();

	timer_ -= kDeltaTime_;
	// 経過時間をスイッチ時間で割る
	float time = timer_ / kPhaseSwitchTime_;

	if (timer_ <= 0.0f)
	{
		timer_ = 0.0f;
		isCompleted_ = true;	//敵の出現が完了
	}

	// イージングで変化させる
	Vector3 scale = Lerp(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), EaseInCubic(time));

	for (WorldTransform& worldTransform : worldTransforms_)
	{
		// スケールをイージングで変化させる
		worldTransform.scale_ = scale;
		worldTransform.UpdateMatrix();
		// ワールドトランスフォームをインスタンシングオブジェクトに追加
		objects_->WorldTransformUpdate(worldTransform);
	}

}

void EnemySpawnObject::SpawnEnemies()
{
	for (const EnemySpawn& spawnData : spawnDatas_)
	{
		//敵生成
		enemyManager_->PopEnemy(spawnData);
	}
	
}
