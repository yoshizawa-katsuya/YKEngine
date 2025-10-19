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

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

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
	/// プレイヤーの弾を追加。
	/// </summary>
	/// <param name="worldPosition">弾のワールド座標</param>
	/// <param name="velocity">弾の速度ベクトル</param>
	/// <param name="bulletType">弾の種類</param>
	void AddPlayerBullet(const Vector3& worldPosition, const Vector3& velocity, PlayerBulletType bulletType);

	/// <summary>
	/// 弾をコリジョンマネージャーに登録。
	/// </summary>
	/// <param name="collisionManager">コリジョンマネージャー</param>
	void RegisterToCollisionManager(CollisionManager* collisionManager);

private:

	//弾のモデル	
	std::shared_ptr<BaseModel> modelBullet_;

	//弾のテクスチャハンドル
	uint32_t textureHandlePlayerBullet_;

	//弾
	std::list<std::unique_ptr<BasePlayerBullet>> playerBullets_;
};

