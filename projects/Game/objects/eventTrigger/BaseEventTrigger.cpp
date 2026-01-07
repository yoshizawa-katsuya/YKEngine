#include "BaseEventTrigger.h"

using namespace YKEngine;

void BaseEventTrigger::Initialize(const YKEngine::ObjectData& objectData)
{
	Collider::Initialize();
	// 半径を設定
	SetRadius(objectData.transform.scale.x);
	// 位置を設定
	worldTransform_.translation_ = objectData.transform.translation;
	worldTransform_.UpdateMatrix();

	typeID_ = CollisionTypeIdDef::kEvent;

	waveNumber_ = objectData.waveNum.value();
}
