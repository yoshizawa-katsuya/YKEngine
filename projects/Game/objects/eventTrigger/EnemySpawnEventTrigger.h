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
	/// <param name="objectData">オブジェクトデータ</param>
	void Initialize(const YKEngine::ObjectData& objectData) override;

	uint32_t GetWaveNumber() const { return waveNumber_; }

private:

	uint32_t waveNumber_ = 0; // ウェーブ番号

};

