#pragma once
#include <BaseState.hpp>
class PlayerStateContext;

/// <summary>
/// プレイヤーのゲームオーバー状態。
/// </summary>
class PlayerGameOverState : public YKEngine::BaseState<PlayerStateContext>
{
	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void OnEnter(PlayerStateContext* player) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void OnUpdate(PlayerStateContext* player) override;
};

