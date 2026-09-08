#pragma once
#include "BasePlayer.h"

/// <summary>
/// 矢印キーで操作するプレイヤークラス
/// </summary>
class RightPlayer : public BasePlayer
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="hp">体力</param>
	void Initialize(YKEngine::BaseModel* model, int32_t* hp) override;

private:

	void Move() override;

};