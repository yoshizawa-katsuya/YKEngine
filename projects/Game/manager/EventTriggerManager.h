#pragma once
#include "eventTrigger/BaseEventTrigger.h"
#include <list>
#include <memory>
#include "LevelDataLoader.h"
class CollisionManager;

/// <summary>
/// イベントトリガーを管理するクラス。
/// イベントトリガーの更新やコリジョンマネージャーへの登録を行う。
/// </summary>
class EventTriggerManager
{
public:

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// イベントトリガーをコリジョンマネージャーに登録する。
	/// </summary>
	/// <param name="collisionManager">コリジョンマネージャー</param>
	void RegisterToCollisionManager(CollisionManager* collisionManager);

	/// <summary>
	/// イベントトリガーを追加する。
	/// </summary>
	/// <param name="eventName">イベントの種類</param>
	/// <param name="objectData">イベントトリガーのデータ</param>
	void AddEvent(const std::string& eventName, const ObjectData& objectData);

private:

	//イベント
	std::list<std::unique_ptr<BaseEventTrigger>> events_;
};

