#pragma once
#include "BaseState.hpp"
class RailCameraStateContext;

/// <summary>
/// レールカメラのゲームオーバーの状態。
/// </summary>
class RailCameraGameOverState : public YKEngine::BaseState<RailCameraStateContext>
{
private:

	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="railCamera">レールカメラ</param>
	void OnEnter(RailCameraStateContext* railCamera) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="railCamera">レールカメラ</param>
	void OnUpdate(RailCameraStateContext* railCamera) override;
};

