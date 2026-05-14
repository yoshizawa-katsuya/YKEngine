#pragma once
#include <BaseState.hpp>
class GameSceneStateContext;

/// <summary>
/// ゲームシーンのポーズ状態。
/// </summary>
class GameScenePauseState : public YKEngine::BaseState<GameSceneStateContext>
{
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

