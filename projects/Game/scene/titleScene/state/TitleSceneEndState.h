#pragma once
#include <BaseState.hpp>
class TitleSceneStateContext;

/// <summary>
/// タイトルシーンの終了状態。
/// </summary>
class TitleSceneEndState : public YKEngine::BaseState<TitleSceneStateContext>
{
private:

	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="titleScene">タイトルシーン</param>
	void OnEnter(TitleSceneStateContext* titleScene) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="titleScene">タイトルシーン</param>
	void OnUpdate(TitleSceneStateContext* titleScene) override;
};

