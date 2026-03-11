#pragma once
#include <BaseState.hpp>
#include <numbers>
class PlayerStateContext;

class PlayerDodgeState : public YKEngine::BaseState<PlayerStateContext>
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

	// 回転速度
	float rotateSpeed_ = std::numbers::pi_v<float> / 8.0f;

	float t_ = 0.0f; // 経過時間
};

