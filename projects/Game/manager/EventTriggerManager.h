#pragma once
#include "eventTrigger/BaseEventTrigger.h"
#include <list>
#include <memory>
#include "LevelDataLoader.h"

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
	/// イベントトリガーを作成する。
	/// </summary>
	void CreateEventTriggers(const std::vector<YKEngine::ObjectData>& objectDatas);

private:

	/// <summary>
	/// イベントトリガーを追加する。
	/// </summary>
	/// <param name="eventName">イベントの種類</param>
	/// <param name="objectData">イベントトリガーのデータ</param>
	void AddEvent(const std::string& eventName, const YKEngine::ObjectData& objectData);

	//イベント
	std::list<std::unique_ptr<BaseEventTrigger>> events_;

	using EventFactory = std::function<std::unique_ptr<BaseEventTrigger>()>;

	//イベントの種類とイベント生成関数の対応表を取得する。	
	const std::unordered_map<std::string, EventFactory>& GetEventFactoryMap() const;

};

