#pragma once
#include <BaseState.hpp>
class ClearSceneStateContext;

namespace YKEngine
{
	class Input;
}

class ClearSceneMainState : public YKEngine::BaseState<ClearSceneStateContext>
{
private:

	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="clearScene">クリアシーン</param>
	void OnEnter(ClearSceneStateContext* clearScene) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="clearScene">クリアシーン</param>
	void OnUpdate(ClearSceneStateContext* clearScene) override;

	YKEngine::Input* input_ = nullptr;
};

