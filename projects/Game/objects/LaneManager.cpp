#include "LaneManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void LaneManager::Initialize(const std::vector<WallData>& wallDatas)
{
	//レーンの生成
	for (std::unique_ptr<Lane>& lane : lanes_)
	{
		lane = std::make_unique<Lane>();
		lane->Initialize(&isStart_);
	}
	//レーンの回転を設定
	lanes_[static_cast<size_t>(LaneType::kLeft)]->SetRotate({ 0.0f, -1.0f, 0.0f });
	lanes_[static_cast<size_t>(LaneType::kRight)]->SetRotate({ 0.0f, 1.0f, 0.0f });

	for (const WallData& wallData : wallDatas)
	{
		//レーンの種別を取得
		const LaneType laneType = wallData.laneType;
		//レーンに壁を追加
		lanes_[static_cast<size_t>(laneType)]->AddWall(wallData);
	}

}

void LaneManager::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("Lane");
	ImGui::Checkbox("Start Move", &isStart_);
	ImGui::End();

#endif // USE_IMGUI	
	//レーンの更新
	for (std::unique_ptr<Lane>& lane : lanes_)
	{
		lane->Update();
	}
}

void LaneManager::Draw(Camera* camera)
{
	//レーンの描画
	for (std::unique_ptr<Lane>& lane : lanes_)
	{
		lane->Draw(camera);
	}
}
