#pragma once
#include <BaseState.hpp>
class ClearSceneStateContext;

class ClearSceneStartState : public YKEngine::BaseState<ClearSceneStateContext>
{
private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="clearScene">クリアシーン</param>
	void OnUpdate(ClearSceneStateContext* clearScene) override;
};

