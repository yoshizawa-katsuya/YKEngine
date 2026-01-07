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

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

	float GetSpeed() const { return speed_; }

private:

	float speed_ = 0.0f;
};

