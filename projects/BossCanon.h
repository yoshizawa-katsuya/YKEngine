#pragma once
#include "WorldTransform.h"
#include "Base3dObject.h"

#include <memory>

class Boss;
class Player;

/// <summary>
/// 砲撃
/// </summary>
class BossCanon
{
public:
	//-------------基本処理-------------//

	void Initialize(Base3dObject* object, Boss* boss, const Vector3& velocity);

	void Update(Camera* camera);

	void Draw();
	
	//-------------ゲッター・セッター-------------//

	/// <summary>
	/// デスフラグゲッター
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { pPlayer_ = player; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

private:
	// ポインタ
	Player* pPlayer_ = nullptr;
	Boss* pBoss_ = nullptr;
	std::unique_ptr<BaseModel> model_;
	Base3dObject* object_;

	WorldTransform worldTransform_;
	Vector3 velocity_ = {};

	// デスフラグ
	bool isDead_ = false;

	// 寿命＜Frame＞
	static const int32_t kLifeTime_ = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime_;


};

