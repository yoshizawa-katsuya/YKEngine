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

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

protected:

	bool isDead_ = false;
};

