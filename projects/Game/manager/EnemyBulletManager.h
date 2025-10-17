#pragma once
#include "bullet/EnemyBullet.h"
class Camera;
class CollisionManager;

/// <summary>
/// 敵の弾を管理するクラス。
/// 弾の生成、更新、描画を行う。
/// </summary>
class EnemyBulletManager
{
public:

	void Initialize();

	void Update(Camera* railCamera);

	void Draw(Camera* camera);

	void AddEnemyBullet(const Vector3& worldPosition, const Vector3& velocity, Player* target, float speed);

	void RegisterToCollisionManager(CollisionManager* collisionManager);

private:

	//弾のモデル	
	std::shared_ptr<BaseModel> modelBullet_;

	//弾のテクスチャハンドル
	uint32_t textureHandleEnemyBullet_;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
};

