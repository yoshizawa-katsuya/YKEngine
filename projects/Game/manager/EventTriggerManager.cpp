#include "EventTriggerManager.h"
#include "eventTrigger/EnemySpawnEventTrigger.h"
#include "eventTrigger/SpeedEventTrigger.h"
#include "eventTrigger/RotateEventTrigger.h"
#include "eventTrigger/RotateResetEventTrigger.h"
#include "CollisionManager.h"

using namespace YKEngine;

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
	for (std::unique_ptr<BaseEventTrigger>& event : events_) 
	{
		collisionManager->AddCollider(event.get());
	}
}

void EventTriggerManager::CreateEventTriggers(const std::vector<ObjectData>& objectDatas)
{
	for (const ObjectData& objectData : objectDatas)
	{
		std::string fileName = objectData.fileName;

		// "Event" を含むファイル名のみ処理
		if (fileName.find("Event") == std::string::npos)
		{
			continue; // "Event" を含まない場合はスキップ
		}

		// イベントの生成
		AddEvent(fileName, objectData);
	}
}

void EventTriggerManager::AddEvent(const std::string& eventName, const ObjectData& objectData)
{
	if (eventName == "waveEvent")
	{
		//波イベントの生成
		std::unique_ptr<BaseEventTrigger> waveEvent = std::make_unique<EnemySpawnEventTrigger>();
		waveEvent->Initialize(objectData.waveNum.value(), objectData.transform.translation, objectData.transform.scale.x);

		// リストに追加
		events_.push_back(std::move(waveEvent));
	}
	else if (eventName == "speedEvent")
	{
		//スピードイベントの生成
		std::unique_ptr<SpeedEventTrigger> speedEvent = std::make_unique<SpeedEventTrigger>();

		// 派生クラス側のInitializeを呼ぶ
		speedEvent->Initialize(objectData.waveNum.value(), objectData.transform.translation, objectData.transform.scale.x, objectData.speed.value());

		// BaseEventTrigger 側としてリストに追加
		events_.push_back(std::move(speedEvent));
	}
	else if (eventName == "rotateEvent")
	{
		// 回転イベントの生成
		std::unique_ptr<RotateEventTrigger> rotateEvent = std::make_unique<RotateEventTrigger>();

		// 派生クラス側のInitializeを呼ぶ
		rotateEvent->Initialize(objectData.waveNum.value(), objectData.transform.translation, objectData.transform.rotation, objectData.transform.scale.x);

		// BaseEventTrigger 側としてリストに追加
		events_.push_back(std::move(rotateEvent));

	}
	else if (eventName == "rotateResetEvent")
	{
		//回転リセットイベントの生成
		std::unique_ptr<BaseEventTrigger> rotateResetEvent = std::make_unique<RotateResetEventTrigger>();
		rotateResetEvent->Initialize(objectData.waveNum.value(), objectData.transform.translation, objectData.transform.scale.x);

		// リストに追加
		events_.push_back(std::move(rotateResetEvent));
	}
}
