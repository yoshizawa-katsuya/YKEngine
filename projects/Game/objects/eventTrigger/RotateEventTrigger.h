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
	/// <param name="objectData">オブジェクトデータ</param>
	void Initialize(const YKEngine::ObjectData& objectData) override;

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

	const YKEngine::Vector3& GetRotate() const { return worldTransform_.rotation_; }

private:

};