#pragma once
#include "Lane.h"
#include <array>

namespace YKEngine
{
	class GlobalVariables;
}

class LaneManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="wallDatas">壁のデータ</param>
	void Initialize(const std::vector<YKEngine::WallData>& wallDatas);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(YKEngine::Camera* camera);

	// 壁を取得する
	const std::vector<std::unique_ptr<Wall>>& GetWalls(YKEngine::LaneType laneType) const { return lanes_[static_cast<size_t>(laneType)]->GetWalls(); }

private:

	void UpdateLaneAngle();

private:

	//レーン
	std::array<std::unique_ptr<Lane>, static_cast<size_t>(YKEngine::LaneType::kCount)> lanes_;
	
	//流れ始めるかどうか
	bool isStart_ = false;

	YKEngine::GlobalVariables* globalVariables_ = nullptr;
};

