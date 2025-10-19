#pragma once
#include "BaseEnemy.h"
#include "EnemySpawn.h"
class Camera;
class Player;
class EnemyBulletManager;
class CollisionManager;

/// <summary>
/// 敵を管理するクラス。
/// 敵の生成、更新、描画、コリジョンマネージャーへの登録を行う。
/// 
/// </summary>
class EnemyManager
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="player">プレイヤーのポインタ。敵の弾の発射先に使用。</param>
	/// <param name="railCamera">レールカメラのポインタ。カメラに映っているかの判定に使用。</param>
	/// <param name="viewPortMatrix">ビューポート変換行列のポインタ。スクリーン座標への変換に使用。</param>
	/// <param name="enemyBulletManager">敵弾マネージャーのポインタ。敵弾の生成に使用。</param>
	void Initialize(Player* player, Camera* railCamera, Matrix4x4* viewPortMatrix, EnemyBulletManager* enemyBulletManager);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">描画に使用するカメラ</param>
	void Draw(Camera* camera);

	/// <summary>
	/// 敵の生成。
	/// </summary>
	/// <param name="spawnData">生成データ</param>
	void PopEnemy(const EnemySpawn& spawnData);

	/// <summary>
	/// 敵をコリジョンマネージャーに登録。
	/// </summary>
	/// <param name="collisionManager">コリジョンマネージャーのポインタ</param>
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

