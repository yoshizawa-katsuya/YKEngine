#pragma once
#include <BaseState.hpp>
class DemoPlayerStateContext;

/// <summary>
/// デモプレイヤーのメイン状態クラス。
/// </summary>
class DemoPlayerMainState : public YKEngine::BaseState<DemoPlayerStateContext>
{
private:

	/// <summary>
	/// 開始処理
	/// </summary>
	/// <param name="demoPlayer">デモプレイヤー</param>
	void OnEnter(DemoPlayerStateContext* demoPlayer) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="demoPlayer">デモプレイヤー</param>
	void OnUpdate(DemoPlayerStateContext* demoPlayer) override;
};

