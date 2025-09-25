#pragma once
#include "bullet/EnemyBullet.h"
class Camera;
class CollisionManager;

class EnemyBulletManager
{
public:

	void Initialize();

	void Update(Camera* railCamera);

	void Draw(Camera* camera);

	void AddEnemyBullet(const Vector3& worldPosition, const Vector3& velocity);

	void AddColliders(CollisionManager* collisionManager);

private:

	//弾のモデル	
	std::shared_ptr<BaseModel> modelBullet_;

	//弾のテクスチャハンドル
	uint32_t textureHandleEnemyBullet_;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
};

