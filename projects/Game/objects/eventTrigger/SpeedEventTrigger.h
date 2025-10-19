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
	/// <param name="waveNumber">イベント番号。昇順でイベントが発生する。</param>
	/// <param name="position">イベントの位置</param>
	/// <param name="radius">イベントの半径</param>
	/// <param name="speed">変更後の速さ</param>
	void Initialize(uint32_t waveNumber, const Vector3& position, float radius, float speed);

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

	float GetSpeed() const { return speed_; }

private:

	float speed_ = 0.0f;
};

