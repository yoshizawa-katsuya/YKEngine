#include "Lane.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void Lane::Initialize(const std::vector<WallData>& wallDatas)
{
	for (const WallData& wallData : wallDatas) {
		//===== 壁の生成 =====
		std::unique_ptr<Wall> wall = std::make_unique<Wall>();
		wall->Initialize(wallData.Translate);

		walls_.push_back(std::move(wall));
	}
}

void Lane::Update()
{
#ifdef USE_IMGUI

    ImGui::Begin("Lane");
	ImGui::Checkbox("Start Move", &isStart_);
    ImGui::End();

#endif // USE_IMGUI	
	for (const std::unique_ptr<Wall>& wall : walls_)
	{
		wall->Update();
		wall->SetIsStart(isStart_);
	}
}

void Lane::Draw(YKEngine::Camera* camera)
{
	//壁の描画
	for (const std::unique_ptr<Wall>& wall : walls_)
	{
		wall->Draw(camera);
	}
}
