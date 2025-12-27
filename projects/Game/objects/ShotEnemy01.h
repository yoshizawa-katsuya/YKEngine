#pragma once
#include "BaseEnemy.h"
#include "bullet/EnemyBulletType.h"

/// <summary>
/// プレイヤーを狙って弾を撃つ敵キャラクター。
/// 弾は一定間隔で撃つ。
/// </summary>
class ShotEnemy01 : public BaseEnemy
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~ShotEnemy01() override;

	/// <summary>
	/// メイン部の初期化。
	/// </summary>
	void MainInitialize() override;

	/// <summary>
	/// メイン更新。
	/// </summary>
	void UpdateMain() override;

	/// <summary>
	/// 弾の発射。
	/// </summary>
	void Fire();

protected:

	//発射間隔
	const float kFireInterval_ = 1.0f;
	//発射タイマー
	float fireTimer_ = 0.0f;

	//弾の種類
	EnemyBulletType bulleyType_ = EnemyBulletType::kTarget;

};

