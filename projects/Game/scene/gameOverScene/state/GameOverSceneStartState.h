#pragma once
#include <BaseState.hpp>
class GameOverSceneStateContext;

/// <summary>
/// ゲームオーバーシーンの開始状態。
/// </summary>
class GameOverSceneStartState : public YKEngine::BaseState<GameOverSceneStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="gameOverScene">ゲームオーバーシーン</param>
	void OnUpdate(GameOverSceneStateContext* gameOverScene) override;
};

