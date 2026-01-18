#pragma once
#include "bullet/BaseEnemyBullet.h"
#include "bullet/EnemyBulletType.h"
class CollisionManager;

/// <summary>
/// 敵の弾を管理するクラス。
/// 弾の生成、更新、描画を行う。
/// </summary>
class EnemyBulletManager
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新。レールカメラは弾が画面外に出たかの判定に使用する。
	/// </summary>
	/// </ <param name="railCamera">レールカメラ</param>
	void Update(YKEngine::Camera* railCamera);

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">描画に使うカメラ</param>
	void Draw(YKEngine::Camera* camera);

	/// <summary>
	/// 敵の弾を追加する。
	/// </summary>
	/// <param name="worldPosition">弾のワールド座標</param>
	/// <param name="velocity">弾の速度ベクトル</param>
	/// <param name="target">弾のターゲットとなるプレイヤー</param>
	/// <param name="speed">弾の速度</param>
	void AddEnemyBullet(const YKEngine::Vector3& worldPosition, const YKEngine::Vector3& velocity, Player* target, float speed, EnemyBulletType bulletType);

	/// <summary>
	/// コライダーを衝突管理クラスに登録する。
	/// </summary>
	/// <param name="collisionManager">衝突管理クラス</param>
	void RegisterToCollisionManager(CollisionManager* collisionManager);

	void SetIsGameOver(bool isGameOver) { isGameOver_ = isGameOver; }

private:

	//敵のモデル
	std::map<EnemyBulletType, std::shared_ptr<YKEngine::BaseModel>> modelEnemyBulletMap_;

	//弾
	std::list<std::unique_ptr<BaseEnemyBullet>> enemyBullets_;

	//ゲームオーバーならtrue
	bool isGameOver_ = false;
};

