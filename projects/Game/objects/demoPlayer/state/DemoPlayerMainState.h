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
	/// 更新処理
	/// </summary>
	/// <param name="demoPlayer">デモプレイヤー</param>
	void OnUpdate(DemoPlayerStateContext* demoPlayer) override;
};

