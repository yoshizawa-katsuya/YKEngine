#pragma once
#include <BaseState.hpp>
class GameOverSceneStateContext;

namespace YKEngine
{
	class Input;
}

class GameOverSceneMainState : public YKEngine::BaseState<GameOverSceneStateContext>
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameOverSceneMainState();

private:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="gameOverScene">ゲームオーバーシーン</param>
	void OnUpdate(GameOverSceneStateContext* gameOverScene) override;

	YKEngine::Input* input_;
};

