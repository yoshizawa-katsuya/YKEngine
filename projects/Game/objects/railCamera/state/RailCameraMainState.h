#pragma once
#include "BaseState.hpp"
#include "RailCameraStateContext.h"

class RailCameraMainState : public YKEngine::BaseState<RailCameraStateContext>
{
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="railCamera">レールカメラ</param>
	void OnUpdate(RailCameraStateContext* railCamera) override;
};

