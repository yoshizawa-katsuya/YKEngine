#pragma once
#include "BaseState.hpp"
class EnemySpawnObjectStateContext;

/// <summary>
/// 敵スポーンオブジェクトの中間状態クラス。
/// </summary>
class EnemySpawnObjectIntervalState : public YKEngine::BaseState<EnemySpawnObjectStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="enemySpawnObject">敵スポーンオブジェクト</param>
	void OnUpdate(EnemySpawnObjectStateContext* enemySpawnObject) override;

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="enemySpawnObject">敵スポーンオブジェクト</param>
	void OnExit(EnemySpawnObjectStateContext* enemySpawnObject) override;
};

