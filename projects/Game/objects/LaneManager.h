#pragma once
#include "Lane.h"
#include <array>
#include "WallModels.h"

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
	const std::vector<std::unique_ptr<Wall>>& GetWalls(PlayerDirection laneType) const { return lanes_[static_cast<size_t>(laneType)]->GetWalls(); }

	/// <summary>
	/// レーンの最後の壁がプレイヤーを通り過ぎていたらtrue、まだならfalseを更新する
	/// </summary>
	void CheckIsEnd();

	bool GetIsEnd() const { return isEnd_; }

private:

	void UpdateLaneAngle();

private:

	//壁のモデル
	std::unique_ptr<WallModels> wallModels_;

	//レーン
	std::array<std::unique_ptr<Lane>, static_cast<size_t>(PlayerDirection::Count)> lanes_;

	//流れ始めるかどうか
	bool isStart_ = true;

	bool isEnd_ = false; //レーンの最後の壁がプレイヤーを通り過ぎていたらtrue、まだならfalse

	YKEngine::GlobalVariables* globalVariables_ = nullptr;
};

