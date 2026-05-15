#include "LaneManager.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void LaneManager::Initialize(const std::vector<WallData>& wallDatas)
{
	//GlovalVariablesに登録
	globalVariables_ = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Lane::kGroupName;
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, JsonKey::Lane::kLaneAngle, 1.0f);

	//レーンの生成
	for (std::unique_ptr<Lane>& lane : lanes_)
	{
		lane = std::make_unique<Lane>();
		lane->Initialize(&isStart_);
	}
	//レーンの回転を設定
	UpdateLaneAngle();

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

#ifdef _DEBUG
	//レーンの角度の更新
	UpdateLaneAngle();
#endif // _DEBUG

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

void LaneManager::UpdateLaneAngle()
{
	//レーンの回転を設定
	Vector3 rotate = {0.0f, globalVariables_->GetFloatValue(JsonKey::Lane::kGroupName, JsonKey::Lane::kLaneAngle), 0.0f};

	lanes_[static_cast<size_t>(LaneType::kLeft)]->SetRotate(-rotate);
	lanes_[static_cast<size_t>(LaneType::kRight)]->SetRotate(rotate);
}
