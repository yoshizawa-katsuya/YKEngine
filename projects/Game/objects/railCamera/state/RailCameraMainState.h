#pragma once
#include "BaseState.hpp"
class RailCameraStateContext;

class RailCameraMainState : public YKEngine::BaseState<RailCameraStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="railCamera">レールカメラ</param>
	void OnUpdate(RailCameraStateContext* railCamera) override;
};

