#pragma once
#include <BaseState.hpp>
class EnemyStateContext;

/// <summary>
/// 敵のメイン状態。
/// </summary>
class EnemyMainState : public YKEngine::BaseState<EnemyStateContext>
{
private:

	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="enemy">敵</param>
	void OnEnter(EnemyStateContext* enemy) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="enemy">敵</param>
	void OnUpdate(EnemyStateContext* enemy) override;
};

