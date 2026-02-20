#pragma once
#include <BaseState.hpp>
class EnemyStateContext;

class EnemyApproachState : public YKEngine::BaseState<EnemyStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="enemy">敵</param>
	void OnUpdate(EnemyStateContext* enemy) override;
};

