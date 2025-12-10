#pragma once
#include "BaseEnemyBullet.h"

/// <summary>
/// 自機を追尾する敵弾01
/// </summary>
class EnemyHomingBullet01 : public BaseEnemyBullet
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期位置</param>
	/// <param name="velocity">初速度</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="target">ホーミングターゲット</param>
	/// <param name="speed">速さ</param>
	void Initialize(YKEngine::BaseModel* model, const YKEngine::Vector3& position, const YKEngine::Vector3& velocity, Player* target, float speed) override;

private:

	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move() override;

	/// <summary>
	/// ターゲットに向かって追尾する。
	/// </summary>
	void Homig();

	Player* target_ = nullptr;
	bool isHoming_ = true; // ホーミング弾かどうか

};

