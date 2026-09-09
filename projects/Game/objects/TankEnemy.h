#pragma once
#include "BaseEnemy.h"
class TankEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// コンストラクタ。
	/// </summary>
	TankEnemy() { hp_ = kMaxHp_; }

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="spawnData">敵の生成データ</param>
	/// <param name="target1">ターゲット1</param>
	/// <param name="target2">ターゲット2</param>
	void Initialize(const YKEngine::EnemySpawnData& spawnData, const BasePlayer* target1, const BasePlayer* target2) override;

private:

	void Move() override;

private:

	const YKEngine::Vector4 kColor_ = { 0.0f, 1.0f, 0.0f, 1.0f };

	const float kMaxHp_ = 10.0f;

};

