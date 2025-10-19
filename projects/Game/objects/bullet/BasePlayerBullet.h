#pragma once
#include "BaseBullet.h"

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
	/// <param name="velocity">初速度</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	virtual void Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) override;

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

};