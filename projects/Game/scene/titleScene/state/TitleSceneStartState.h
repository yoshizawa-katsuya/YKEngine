#pragma once
#include <BaseState.hpp>
class TitleSceneStateContext;

/// <summary>
/// タイトルシーンの開始状態。
/// </summary>
class TitleSceneStartState : public YKEngine::BaseState<TitleSceneStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="titleScene">タイトルシーン</param>
	void OnUpdate(TitleSceneStateContext* titleScene) override;
};

