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

	const YKEngine::Vector3& GetRotate() const { return worldTransform_.rotation_; }

private:

};