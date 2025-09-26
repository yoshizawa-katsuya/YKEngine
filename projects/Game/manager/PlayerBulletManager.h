#pragma once
#include "bullet/PlayerBullet.h"
class Camera;
class CollisionManager;

class PlayerBulletManager
{
public:

	void Initialize();

	void Update();

	void Draw(Camera* camera);

	void AddPlayerBullet(const Vector3& worldPosition, const Vector3& velocity);

	void RegisterToCollisionManager(CollisionManager* collisionManager);

private:

	//弾のモデル	
	std::shared_ptr<BaseModel> modelBullet_;

	//弾のテクスチャハンドル
	uint32_t textureHandlePlayerBullet_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;
};

