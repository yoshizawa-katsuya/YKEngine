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
		collisionManager->AddSphereCollider(event.get());
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
	const std::unordered_map<std::string, EventTriggerManager::EventFactory>& eventFactoryMap = GetEventFactoryMap();

	auto it = eventFactoryMap.find(eventName);
	if (it == eventFactoryMap.end())
	{
		assert(false); // イベントの種類が見つからない場合はエラー
		return;
	}

	std::unique_ptr<BaseEventTrigger> eventTrigger = it->second();

	// 初期化
	eventTrigger->Initialize(objectData);
	// リストに追加
	events_.push_back(std::move(eventTrigger));
}

const std::unordered_map<std::string, EventTriggerManager::EventFactory>& EventTriggerManager::GetEventFactoryMap() const
{
	static const std::unordered_map<std::string, EventFactory> eventFactoryMap =
	{
		{"waveEvent", []() { return std::make_unique<EnemySpawnEventTrigger>(); }},
		{"speedEvent", []() { return std::make_unique<SpeedEventTrigger>(); }},
		{"rotateEvent", []() { return std::make_unique<RotateEventTrigger>(); }},
		{"rotateResetEvent", []() { return std::make_unique<RotateResetEventTrigger>(); }}
	};
	return eventFactoryMap;
}
