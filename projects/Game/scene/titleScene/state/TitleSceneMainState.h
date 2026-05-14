#pragma once
#include <BaseState.hpp>
class TitleSceneStateContext;

namespace YKEngine
{
	class Input;
}

/// <summary>
/// タイトルシーンのメインの状態。
/// </summary>
class TitleSceneMainState : public YKEngine::BaseState<TitleSceneStateContext>
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleSceneMainState();

private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="titleScene">タイトルシーン</param>
	void OnUpdate(TitleSceneStateContext* titleScene) override;

	YKEngine::Input* input_;
};

