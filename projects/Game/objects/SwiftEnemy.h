#pragma once
#include "BaseEnemy.h"
class SwiftEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="spawnData">敵の生成データ</param>
	/// <param name="target1">ターゲット1</param>
	/// <param name="target2">ターゲット2</param>
	void Initialize(const YKEngine::EnemySpawnData& spawnData, const BasePlayer* target1, const BasePlayer* target2) override;

	/// <summary>
	/// コンストラクタ。
	/// </summary>
	SwiftEnemy() { hp_ = kMaxHp_; }

private:

	/// <summary>
	///  
	/// </summary>
	void Move() override;

	YKEngine::Vector3 CalculateCenterPosition();

	YKEngine::Vector3 RotateTowards(const YKEngine::Vector3& targetPosition, float deltaTime = 1.0f / 60.0f);

private:

	const YKEngine::Vector4 kColor_ = { 0.2f, 0.8f, 0.99f, 1.0f };

	const float kMaxHp_ = 5.0f;

	// 最大ホーミング角度
	const float kMaxHomingAngle_ = 5.0f;

	// 目的地に到達したかどうか
	bool hasReachedDestination_ = false;
};

