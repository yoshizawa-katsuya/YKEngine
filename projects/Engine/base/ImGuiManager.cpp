#include "ImGuiManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"

void ImGuiManager::Initialize(DirectXCommon* dxCommon, WinApp* winApp, SrvHeapManager* srvHeapManager)
{

	dxCommon_ = dxCommon;
	uint32_t index = srvHeapManager->Allocate();

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

}

void ImGuiManager::Begin()
{

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void ImGuiManager::End()
{

	ImGui::Render();

}

void ImGuiManager::Draw()
{

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon_->GetCommandList());

}

void ImGuiManager::Finalize()
{

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}
