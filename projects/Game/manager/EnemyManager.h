#pragma once
#include "BaseEnemy.h"
#include "EnemySpawn.h"
#include "EnemyParams.h"
class Player;
class EnemyBulletManager;

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
	/// <param name="enemyBulletManager">敵弾マネージャーのポインタ。敵弾の生成に使用。</param>
	void Initialize(Player* player, YKEngine::Camera* railCamera, EnemyBulletManager* enemyBulletManager);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">描画に使用するカメラ</param>
	void Draw(YKEngine::Camera* camera);

	/// <summary>
	/// 敵の生成。
	/// </summary>
	/// <param name="spawnData">生成データ</param>
	void PopEnemy(const EnemySpawn& spawnData);

	const std::list<std::unique_ptr<BaseEnemy>>& GetEnemies() { return enemies_; }

private:

	/// <summary>
	/// パラメータのセットアップ。
	/// </summary>
	void ParamsSetup();

	//敵のモデル
	std::map<EnemyType, std::shared_ptr<YKEngine::BaseModel>> modelEnemyMap_;

	std::map<EnemyType, ShotEnemyParams> shotEnemyParamsMap_;

	//敵のリスト
	std::list<std::unique_ptr<BaseEnemy>> enemies_;

	//プレイヤーのポインタ
	Player* player_ = nullptr;

	//レールカメラのポインタ
	YKEngine::Camera* railCamera_ = nullptr;

	//敵弾マネージャーのポインタ
	EnemyBulletManager* enemyBulletManager_ = nullptr;

	using EnemyFactory = std::function<std::unique_ptr<BaseEnemy>(EnemyManager*, const EnemySpawn&)>;

	const std::unordered_map<EnemyType, EnemyFactory>& GetEnemyFactoryMap() const;

};

