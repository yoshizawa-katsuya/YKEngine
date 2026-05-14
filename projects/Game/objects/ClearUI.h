#pragma once
#include <memory>
#include "PressA.h"

namespace YKEngine
{
	class Sprite;
}

/// <summary>
/// クリアシーンのUIクラス
/// </summary>
class ClearUI
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

	std::unique_ptr<YKEngine::Sprite> spriteClear_;//ステージクリアのスプライト

	std::unique_ptr<PressA> pressA_;//Aボタンを押してくださいのUI
};
