#include "ImGuiManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
#endif // USE_IMGUI

using namespace YKEngine;

ImGuiManager::~ImGuiManager()
{
	Finalize();
}

void ImGuiManager::Initialize(DirectXCommon* dxCommon, WinApp* winApp, SrvHeapManager* srvHeapManager)
{

	dxCommon_ = dxCommon;
	uint32_t index = srvHeapManager->Allocate();

#ifdef USE_IMGUI
	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(dxCommon_->GetDevice(),
		//swapChainDesc.BufferCount,
		static_cast<int>(dxCommon_->GetBackBufferCount()),
		//rtvDesc.Format,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeapManager->GetDescriptorHeap(),
		srvHeapManager->GetCPUDescriptorHandle(index),
		srvHeapManager->GetGPUDescriptorHandle(index));
#endif // USE_IMGUI

}

void ImGuiManager::Begin()
{
#ifdef USE_IMGUI
	//ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif // USE_IMGUI

}

void ImGuiManager::End()
{
#ifdef USE_IMGUI
	//ImGuiフレーム終了
	ImGui::Render();
#endif // USE_IMGUI

}

void ImGuiManager::Draw()
{
#ifdef USE_IMGUI
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon_->GetCommandList());
#endif // USE_IMGUI

}

void ImGuiManager::Finalize()
{

#ifdef USE_IMGUI
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // USE_IMGUI

}
