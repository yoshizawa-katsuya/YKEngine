#include "EventTriggerManager.h"
#include "eventTrigger/EnemySpawnEventTrigger.h"
#include "eventTrigger/SpeedEventTrigger.h"
#include "eventTrigger/RotateEventTrigger.h"
#include "eventTrigger/RotateResetEventTrigger.h"
#include "CollisionManager.h"

void EventTriggerManager::Update()
{
	//デスフラグの立ったイベントを削除
	events_.remove_if([](std::unique_ptr<BaseEventTrigger>& event) {
		if (event->IsDead()) {
			return true;
		}
		return false;
		});
}

void EventTriggerManager::RegisterToCollisionManager(CollisionManager* collisionManager)
{
	for (std::unique_ptr<BaseEventTrigger>& event : events_) {
		collisionManager->AddCollider(event.get());
	}
}

void EventTriggerManager::AddEvent(std::string eventName, const ObjectData& objectData)
{
	if (eventName == "waveEvent")
	{
		//波イベントの生成
		std::unique_ptr<BaseEventTrigger>& waveEvent = events_.emplace_back();
		waveEvent = std::make_unique<EnemySpawnEventTrigger>();
		waveEvent->Initialize(objectData.waveNum.value(), objectData.transform.translation, objectData.transform.scale.x);

	}
	else if (eventName == "speedEvent")
	{
		//スピードイベントの生成
		std::unique_ptr<BaseEventTrigger>& speedEvent = events_.emplace_back();
		SpeedEventTrigger* speedEventPtr = new SpeedEventTrigger();
		speedEventPtr->Initialize(objectData.waveNum.value(), objectData.transform.translation, objectData.transform.scale.x, objectData.speed.value());
		speedEvent = std::make_unique<SpeedEventTrigger>(*speedEventPtr);

	}
	else if (eventName == "rotateEvent")
	{
		//回転イベントの生成
		std::unique_ptr<BaseEventTrigger>& rotateEvent = events_.emplace_back();
		RotateEventTrigger* rotateEventPtr = new RotateEventTrigger();
		rotateEventPtr->Initialize(objectData.waveNum.value(), objectData.transform.translation, objectData.transform.rotation, objectData.transform.scale.x);
		rotateEvent = std::make_unique<RotateEventTrigger>(*rotateEventPtr);

	}
	else if (eventName == "rotateResetEvent")
	{
		//回転リセットイベントの生成
		std::unique_ptr<BaseEventTrigger>& rotateResetEvent = events_.emplace_back();
		rotateResetEvent = std::make_unique<RotateResetEventTrigger>();
		rotateResetEvent->Initialize(objectData.waveNum.value(), objectData.transform.translation, objectData.transform.scale.x);

	}
}
