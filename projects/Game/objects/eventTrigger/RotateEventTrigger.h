#pragma once
#include "BaseEventTrigger.h"

/// <summary>
/// 回転イベントトリガー。
/// RailMoverが触れたとき、回転させる。
/// </summary>
class RotateEventTrigger : public BaseEventTrigger
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="waveNumber">イベント番号。昇順でイベントが発生する。</param>
	/// <param name="position">イベントの位置</param>
	/// <param name="radius">イベントの半径</param>
	void Initialize(uint32_t waveNumber, const Vector3& position, const Vector3& rotate, float radius);

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

	const Vector3& GetRotate() const { return worldTransform_.rotation_; }

private:

};