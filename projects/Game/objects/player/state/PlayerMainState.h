#pragma once
#include <BaseState.hpp>
class PlayerStateContext;

class PlayerMainState : public YKEngine::BaseState<PlayerStateContext>
{
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void OnUpdate(PlayerStateContext* player) override;

};

