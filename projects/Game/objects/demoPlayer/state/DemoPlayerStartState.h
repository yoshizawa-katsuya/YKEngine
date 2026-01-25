#pragma once
#include <BaseState.hpp>
class DemoPlayerStateContext;

class DemoPlayerStartState : public YKEngine::BaseState<DemoPlayerStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="demoPlayer">デモプレイヤー</param>
	void OnUpdate(DemoPlayerStateContext* demoPlayer) override;

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="demoPlayer">デモプレイヤー</param>
	void OnExit(DemoPlayerStateContext* demoPlayer) override;

};

