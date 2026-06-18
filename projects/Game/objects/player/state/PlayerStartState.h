#pragma once
#include <BaseState.hpp>
class PlayerStateContext;

/// <summary>
/// プレイヤーの開始部の状態。
/// </summary>
class PlayerStartState : public YKEngine::BaseState<PlayerStateContext>
{
private:

	/// <summary>
	/// 開始処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void OnEnter(PlayerStateContext* player) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void OnUpdate(PlayerStateContext* player) override;

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void OnExit(PlayerStateContext* player) override;

};

