#pragma once
#include <BaseState.hpp>
class GameSceneStateContext;

/// <summary>
/// ゲームシーンのメインの状態。
/// </summary>
class GameSceneMainState : public YKEngine::BaseState<GameSceneStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="gameScene">ゲームシーン</param>
	void OnUpdate(GameSceneStateContext* gameScene) override;
};

