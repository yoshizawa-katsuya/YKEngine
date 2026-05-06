#pragma once
#include "bullet/BasePlayerBullet.h"
#include "bullet/PlayerBulletType.h"
class BaseEnemy;

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
	void Draw(YKEngine::Camera* camera);

	/// <summary>
	/// プレイヤーの弾を追加。
	/// </summary>
	/// <param name="worldPosition">弾のワールド座標</param>
	/// <param name="direction">方向</param>
	/// <param name="bulletType">弾の種類</param>
	/// <param name="speed">弾の速度</param>
	void AddPlayerBullet(const YKEngine::Vector3& worldPosition, const YKEngine::Vector3& direction, PlayerBulletType bulletType, BaseEnemy* targetEnemy);

private:

	//弾のモデル	
	std::shared_ptr<YKEngine::BaseModel> modelBullet_;

	//弾
	std::list<std::unique_ptr<BasePlayerBullet>> playerBullets_;

	using PlayerBulletFactory = std::function<std::unique_ptr<BasePlayerBullet>()>;

	/// <summary>
	/// プレイヤーの弾の種類とプレイヤーの弾生成関数の対応表を取得する。
	/// </summary>
	/// <returns>プレイヤーの弾の種類とプレイヤーの弾生成関数の対応表</returns>
	const std::unordered_map<PlayerBulletType, PlayerBulletFactory>& GetPlayerBulletFactoryMap() const;

};

