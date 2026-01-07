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

	std::unique_ptr<BaseEventTrigger> eventTrigger;

	if (eventName == "waveEvent")
	{
		//敵出現イベントの生成
		eventTrigger = std::make_unique<EnemySpawnEventTrigger>();
	}
	else if (eventName == "speedEvent")
	{
		//スピードイベントの生成
		eventTrigger = std::make_unique<SpeedEventTrigger>();
	}
	else if (eventName == "rotateEvent")
	{
		// 回転イベントの生成
		eventTrigger = std::make_unique<RotateEventTrigger>();
	}
	else if (eventName == "rotateResetEvent")
	{
		//回転リセットイベントの生成
		eventTrigger = std::make_unique<RotateResetEventTrigger>();
	}

	// 初期化
	eventTrigger->Initialize(objectData);
	// リストに追加
	events_.push_back(std::move(eventTrigger));
}
