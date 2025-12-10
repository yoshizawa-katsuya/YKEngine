#pragma once
#include "BaseEnemyBullet.h"

/// <summary>
/// 自機狙い弾01
/// </summary>
class TargetEnemyBullet01 : public BaseEnemyBullet
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

	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="railCamera">レールカメラ。画面外に出たかの判定に使用。</param>
	void Update(YKEngine::Camera* railCamera) override;

private:

	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move() override;

	/// <summary>
	/// ターゲットに向かって追尾する。
	/// </summary>
	void Homig();

	YKEngine::WorldTransform target_;
	bool isHoming_ = true; // ホーミング弾かどうか
	float targetRadius_; // ターゲットの半径

};

