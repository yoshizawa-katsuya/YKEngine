#pragma once
#include "BaseState.hpp"
class EnemySpawnObjectStateContext;

class EnemySpawnObjectEndState : public YKEngine::BaseState<EnemySpawnObjectStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="enemySpawnObject">敵スポーンオブジェクト</param>
	void OnUpdate(EnemySpawnObjectStateContext* enemySpawnObject) override;
};

