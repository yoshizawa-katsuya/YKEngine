#pragma once
#include <map>
#include "InstancingObjects.h"
#include "LevelDataLoader.h"

class StageObjects
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<YKEngine::ObjectData>& objectDatas);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(YKEngine::Camera* camera);

private:

	//オブジェクト
	std::map<std::string, std::unique_ptr<YKEngine::InstancingObjects>> instancingObjects_;

};

