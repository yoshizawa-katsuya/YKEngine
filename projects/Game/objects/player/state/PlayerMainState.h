#pragma once
#include <BaseState.hpp>
class PlayerStateContext;

namespace YKEngine
{
	class Input;
}

/// <summary>
/// プレイヤーのメイン状態クラス。
/// </summary>
class PlayerMainState : public YKEngine::BaseState<PlayerStateContext>
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

	YKEngine::Input* input_ = nullptr;

};

