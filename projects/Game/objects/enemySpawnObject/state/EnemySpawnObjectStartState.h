#pragma once
#include "BaseState.hpp"
class EnemySpawnObjectStateContext;

/// <summary>
/// 敵スポーンオブジェクトの開始状態。
/// </summary>
class EnemySpawnObjectStartState : public YKEngine::BaseState<EnemySpawnObjectStateContext>
{
private:

	/// <summary>
	/// 開始処理
	/// </summary>
	/// <param name="enemySpawnObject">敵スポーンオブジェクト</param>
	void OnEnter(EnemySpawnObjectStateContext* enemySpawnObject) override;

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

