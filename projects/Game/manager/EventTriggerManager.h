#pragma once
#include "eventTrigger/BaseEventTrigger.h"
#include <list>
#include <memory>
#include "LevelDataLoader.h"
class CollisionManager;

class EventTriggerManager
{
public:

	//更新
	void Update();

	//コリジョンマネージャーに登録
	void RegisterToCollisionManager(CollisionManager* collisionManager);

	//イベント追加
	void AddEvent(std::string eventName, const ObjectData& objectData);

private:

	//イベント
	std::list<std::unique_ptr<BaseEventTrigger>> events_;
};

