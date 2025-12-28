#pragma once
#include "TackleEnemy01.h"

/// <summary>
/// プレイヤーに向かって突進する敵キャラ。
/// 突進中はプレイヤーを追尾する。
/// </summary>
class TackleEnemy02 : public TackleEnemy01
{
public:

private:

	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move() override;

};

