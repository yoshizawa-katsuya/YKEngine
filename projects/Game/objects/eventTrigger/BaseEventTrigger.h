#pragma once
#include "Collider.h"
#include "LevelDataLoader.h"

/// <summary>
/// イベントトリガーの基底クラス。
/// RailMoverが通過したときにイベントを発生させる。
/// </summary>
class BaseEventTrigger : public Collider
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	virtual void Initialize(const YKEngine::ObjectData& objectData);

	bool IsDead() { return isDead_; }

	uint32_t GetWaveNumber() const { return waveNumber_; }

protected:

	uint32_t waveNumber_ = 0;

	bool isDead_ = false;
};

