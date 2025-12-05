#pragma once
#include "BaseEventTrigger.h"

/// <summary>
/// 敵出現イベントトリガー。
/// RailMoverが一定範囲に入ると敵が出現する。
/// </summary>
class EnemySpawnEventTrigger : public BaseEventTrigger
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="waveNumber">イベント番号。昇順でイベントが発生する。</param>
	/// <param name="position">イベントの位置</param>
	/// <param name="radius">イベントの半径</param>
	void Initialize(uint32_t waveNumber, const YKEngine::Vector3& position, float radius) override;

	/// <summary>
	/// 衝突時処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

};

