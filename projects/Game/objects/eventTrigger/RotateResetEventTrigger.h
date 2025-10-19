#pragma once
#include "BaseEventTrigger.h"

/// <summary>
/// 回転リセットイベントトリガー。
/// 回転イベントのリセットを行う。
/// 回転イベントの詳細はRotateEventTriggerを参照。
/// </summary>
class RotateResetEventTrigger : public BaseEventTrigger
{
public:

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

};

