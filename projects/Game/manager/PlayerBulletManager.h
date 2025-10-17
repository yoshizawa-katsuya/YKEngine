#pragma once
#include "bullet/BasePlayerBullet.h"
#include "bullet/PlayerBulletType.h"
class Camera;
class CollisionManager;

/// <summary>
/// プレイヤーの弾を管理するクラス。
/// プレイヤーの弾の生成、更新、描画を行う。
/// また、弾のコライダー登録も行う。
/// </summary>
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
	void AddPlayerBullet(const Vector3& worldPosition, const Vector3& velocity, PlayerBulletType bulletType);

	//コライダー登録
	void RegisterToCollisionManager(CollisionManager* collisionManager);

private:

	//弾のモデル	
	std::shared_ptr<BaseModel> modelBullet_;

	//弾のテクスチャハンドル
	uint32_t textureHandlePlayerBullet_;

	//弾
	std::list<std::unique_ptr<BasePlayerBullet>> playerBullets_;
};

