#pragma once
#include <BaseState.hpp>
class GameSceneStateContext;

/// <summary>
/// ゲームシーンのゲームオーバー状態。
/// </summary>
class GameSceneGameOverState : public YKEngine::BaseState<GameSceneStateContext>
{
private:

	/// <summary>
	/// 開始処理
	/// </summary>
	/// <param name="gameScene">ゲームシーン</param>
	void OnEnter(GameSceneStateContext* gameScene) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="gameScene">ゲームシーン</param>
	void OnUpdate(GameSceneStateContext* gameScene) override;
};

