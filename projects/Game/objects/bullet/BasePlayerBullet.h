#pragma once
#include "BaseBullet.h"

class BaseEnemy;

/// <summary>
/// プレイヤー弾の基底クラス。
/// プレイヤー弾の種類ごとにクラスを作成し、BasePlayerBulletを継承して実装する。
/// </summary>
class BasePlayerBullet : public BaseBullet
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期位置</param>
	/// <param name="direction">方向</param>
	/// <param name="targetEnemy">目標の敵オブジェクト</param>
	/// <param name="speed">速度</param>
	virtual void Initialize(YKEngine::BaseModel* model, const YKEngine::Vector3& position, const YKEngine::Vector3& direction, BaseEnemy* targetEnemy);

	/// <summary>
	/// 更新。
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] BaseCollider* other) override;

private:
	
	BaseEnemy* targetEnemy_ = nullptr; // 目標の敵オブジェクト。

	float speed_ = 2.0f;	// 速度。
};