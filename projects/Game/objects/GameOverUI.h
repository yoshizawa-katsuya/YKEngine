#pragma once
#include <memory>
#include "PressA.h"

namespace YKEngine
{
	class Sprite;
}

/// <summary>
/// ゲームオーバーシーンのUIクラス
/// </summary>
class GameOverUI
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:


	/// <summary>
	/// UIの位置を設定
	/// </summary>
	void SetUIPosition();

	std::unique_ptr<YKEngine::Sprite> spriteGameOver_;//ゲームオーバーのスプライト

	std::unique_ptr<PressA> pressA_;//Aボタンを押してくださいのUI

};

