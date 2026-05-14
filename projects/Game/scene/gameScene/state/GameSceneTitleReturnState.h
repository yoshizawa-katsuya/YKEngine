#pragma once
#include <BaseState.hpp>
class GameSceneStateContext;

/// <summary>
/// ゲームシーンのタイトルへ戻る状態。
/// </summary>
class GameSceneTitleReturnState : public YKEngine::BaseState<GameSceneStateContext>
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

