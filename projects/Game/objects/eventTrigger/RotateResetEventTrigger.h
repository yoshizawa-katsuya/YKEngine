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

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

};

