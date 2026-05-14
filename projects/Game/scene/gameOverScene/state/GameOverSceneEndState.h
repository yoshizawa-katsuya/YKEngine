#pragma once
#include <BaseState.hpp>
class GameOverSceneStateContext;

/// <summary>
/// ゲームオーバーシーンの終了状態。
/// </summary>
class GameOverSceneEndState : public YKEngine::BaseState<GameOverSceneStateContext>
{
private:

	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="gameOverScene">ゲームオーバーシーン</param>
	void OnEnter(GameOverSceneStateContext* gameOverScene) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="gameOverScene">ゲームオーバーシーン</param>
	void OnUpdate(GameOverSceneStateContext* gameOverScene) override;
};

