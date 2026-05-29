#pragma once
#include <unordered_map>
#include <memory>
#include "GameType.h"
#include "BaseModel.h"

class WallModels
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	const std::shared_ptr<YKEngine::BaseModel>& GetWallModel(PlayerPose pose) const { return wallModels_.at(pose); }

private:
	
	//壁のモデル
	std::unordered_map<PlayerPose, std::shared_ptr<YKEngine::BaseModel>> wallModels_;
};

