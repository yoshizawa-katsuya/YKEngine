#pragma once
#include <BaseState.hpp>
#include <numbers>
#include "SRTAnimator.h"
class PlayerStateContext;

/// <summary>
/// プレイヤーの回避状態。
/// </summary>
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

	// 回転量（ラジアン）
	float rotateQuantity_ = std::numbers::pi_v<float> * 4.0f;

	std::unique_ptr<YKEngine::SRTAnimator> rotateAnime_;
};

