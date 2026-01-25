#pragma once
#include <BaseState.hpp>
class DemoPlayerStateContext;

class DemoPlayerMainState : public YKEngine::BaseState<DemoPlayerStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="demoPlayer">デモプレイヤー</param>
	void OnUpdate(DemoPlayerStateContext* demoPlayer) override;
};

