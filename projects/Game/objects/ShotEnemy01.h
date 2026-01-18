#pragma once
#include "BaseEnemy.h"
#include "bullet/EnemyBulletType.h"
#include "EnemyParams.h"

/// <summary>
/// 自機狙い弾を撃つ敵キャラクター。
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

	/// <summary>
	/// パラメータの設定。
	/// </summary>
	/// <param name="params">パラメータ。</param>
	void SetParams(ShotEnemyParams params) { params_ = params; }

protected:

	// パラメータ
	ShotEnemyParams params_;

	//発射タイマー
	float fireTimer_ = 0.0f;

};

