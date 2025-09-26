#pragma once
#include "bullet/PlayerBullet.h"
class Camera;
class CollisionManager;

class PlayerBulletManager
{
public:

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw(Camera* camera);

	//弾の追加
	void AddPlayerBullet(const Vector3& worldPosition, const Vector3& velocity);

	//コライダー登録
	void RegisterToCollisionManager(CollisionManager* collisionManager);

private:

	//弾のモデル	
	std::shared_ptr<BaseModel> modelBullet_;

	//弾のテクスチャハンドル
	uint32_t textureHandlePlayerBullet_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;
};

