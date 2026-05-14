#pragma once
#include <BaseState.hpp>
class GameSceneStateContext;

/// <summary>
/// ゲームシーンの開始状態。
/// </summary>
class GameSceneStartState : public YKEngine::BaseState<GameSceneStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="gameScene">ゲームシーン</param>
	void OnUpdate(GameSceneStateContext* gameScene) override;

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="gameScene">ゲームシーン</param>
	void OnExit(GameSceneStateContext* gameScene) override;
	
};

