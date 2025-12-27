#pragma once
#include "ShotEnemy01.h"

/// <summary>
/// 追尾弾を撃つ敵キャラクター。
/// 弾は一定間隔で撃つ。
/// </summary>
class ShotEnemy02 : public ShotEnemy01
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="spawnData">出現データ</param>
	/// <param name="viewPortMatrix">ビューポート行列</param>
	/// <param name="railCamera">レールカメラ。画面内に入っているか判定するために使用。</param>
	void Initialize(YKEngine::BaseModel* model, const EnemySpawn& spawnData, YKEngine::Camera* railCamera) override;
};

