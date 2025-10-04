#pragma once
#include "BaseEnemy.h"
#include "EnemySpawn.h"
class Camera;
class Player;
class EnemyBulletManager;
class CollisionManager;

class EnemyManager
{
public:

	//初期化
	void Initialize(Player* player, Camera* railCamera, Matrix4x4* viewPortMatrix, EnemyBulletManager* enemyBulletManager);

	//更新
	void Update();

	//描画
	void Draw(Camera* camera);

	//敵の生成
	void PopEnemy(const EnemySpawn& spawnData);

	//コライダー登録
	void RegisterToCollisionManager(CollisionManager* collisionManager);

	const std::list<std::unique_ptr<BaseEnemy>>& GetEnemies() { return enemys_; }

private:

	//敵のモデル
	std::map<EnemyType, std::shared_ptr<BaseModel>> modelEnemyMap_;

	//敵のリスト
	std::list<std::unique_ptr<BaseEnemy>> enemys_;

	//プレイヤーのポインタ
	Player* player_ = nullptr;

	//レールカメラのポインタ
	Camera* railCamera_ = nullptr;

	//ビューポート変換行列
	Matrix4x4* viewPortMatrix_ = nullptr;

	//敵弾マネージャーのポインタ
	EnemyBulletManager* enemyBulletManager_ = nullptr;
};

