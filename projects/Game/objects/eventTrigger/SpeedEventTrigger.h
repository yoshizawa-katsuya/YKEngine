#pragma once
#include "BaseEventTrigger.h"

/// <summary>
/// 速度イベントトリガー。
/// RailMoverの速度を変更するイベントトリガー。
/// </summary>
class SpeedEventTrigger : public BaseEventTrigger
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	void Initialize(const YKEngine::ObjectData& objectData) override;

	float GetSpeed() const { return speed_; }

private:

	float speed_ = 0.0f;
};

